"""Tests for cache telemetry record aggregation."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from cache_metrics import (
    aggregate_directory,
    aggregate_records,
    record_from_environment,
    render_markdown,
)


ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / ".github" / "package-manifest.json"


def record(
    job: str,
    sdk_hit: str = "true",
    pip_hit: str = "true",
    fallback: str = "false",
    seconds: int = 4,
    setup: dict | None = None,
) -> dict:
    payload = {
        "schema_version": 1,
        "run_id": "123",
        "commit": "abc",
        "job": job,
        "sdk": {"hit": sdk_hit, "fallback_used": fallback},
        "pip": {"hit": pip_hit},
        "restore_seconds": seconds,
    }
    if setup is not None:
        payload["setup"] = setup
    return payload


class CacheMetricsTests(unittest.TestCase):
    def test_record_includes_setup_telemetry_from_environment(self):
        telemetry = record_from_environment(
            "VIP_A",
            {
                "SYSTEM_SETUP_SECONDS": "7",
                "APT_UPDATED": "false",
                "MISSING_APT_PACKAGES": "libmaxminddb-dev",
                "PIP_INSTALL_SECONDS": "2",
                "PYTHON_VERSION": "3.12.3",
            },
        )

        self.assertEqual(
            telemetry["setup"],
            {
                "system_setup_seconds": 7,
                "apt_updated": False,
                "missing_apt_packages": ["libmaxminddb-dev"],
                "pip_install_seconds": 2,
                "python_version": "3.12.3",
            },
        )

    def test_all_modules_exact(self):
        manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
        modules = sorted(manifest["packages"])
        self.assertEqual(len(modules), 35)
        records = [record("prepare-dependencies")] + [record(module) for module in modules]

        summary = aggregate_records(records, modules)

        self.assertEqual(summary["schema_version"], 1)
        self.assertEqual(summary["actual_module_count"], 35)
        self.assertEqual(summary["sdk"]["exact"], 35)
        self.assertEqual(summary["sdk"]["exact_hit_rate"], 1.0)
        self.assertEqual(summary["pip"]["exact"], 35)
        self.assertEqual(summary["fallback_count"], 0)
        self.assertEqual(summary["restore_seconds"], {"min": 4, "max": 4, "average": 4})
        self.assertEqual(summary["warnings"], [])

    def test_partial_pip_hit_is_counted(self):
        modules = ["VIP_A", "VIP_B"]
        summary = aggregate_records(
            [record("prepare-dependencies"), record("VIP_A"), record("VIP_B", pip_hit="false")],
            modules,
        )

        self.assertEqual(summary["pip"]["exact"], 1)
        self.assertEqual(summary["pip"]["partial"], 1)
        self.assertEqual(summary["pip"]["miss"], 0)
        self.assertIn("pip cache was not exact for VIP_B", summary["warnings"])

    def test_sdk_miss_and_fallback_are_warnings(self):
        summary = aggregate_records(
            [record("prepare-dependencies"), record("VIP_A", sdk_hit="", fallback="true")],
            ["VIP_A"],
        )

        self.assertEqual(summary["sdk"]["miss"], 1)
        self.assertEqual(summary["fallback_count"], 1)
        self.assertTrue(any("SDK cache was not exact" in item for item in summary["warnings"]))
        self.assertTrue(any("fallback clone" in item for item in summary["warnings"]))

    def test_missing_extra_and_malformed_records_are_detected(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            (directory / "prepare.json").write_text(json.dumps(record("prepare-dependencies")), encoding="utf-8")
            (directory / "known.json").write_text(json.dumps(record("VIP_A")), encoding="utf-8")
            (directory / "extra.json").write_text(json.dumps(record("VIP_EXTRA")), encoding="utf-8")
            (directory / "broken.json").write_text("{broken", encoding="utf-8")

            summary = aggregate_directory(directory, ["VIP_A", "VIP_MISSING"])

        self.assertEqual(summary["missing_modules"], ["VIP_MISSING"])
        self.assertEqual(summary["unexpected_modules"], ["VIP_EXTRA"])
        self.assertEqual(summary["malformed_files"], ["broken.json"])
        self.assertTrue(summary["warnings"])

    def test_markdown_contains_aggregate_values(self):
        summary = aggregate_records(
            [record("prepare-dependencies"), record("VIP_A", seconds=2), record("VIP_B", seconds=6)],
            ["VIP_A", "VIP_B"],
        )

        markdown = render_markdown(summary)

        self.assertIn("SDK exact hits: `2/2` (100%)", markdown)
        self.assertIn("Restore window: `2–6s (avg 4s)`", markdown)
        self.assertIn("System setup: `unavailable`", markdown)
        self.assertIn("apt updates: `unavailable`", markdown)
        self.assertIn("Warnings: none", markdown)

    def test_setup_telemetry_is_aggregated(self):
        modules = ["VIP_A", "VIP_B", "VIP_C"]
        setup = [
            {"system_setup_seconds": 2, "apt_updated": False, "missing_apt_packages": [], "pip_install_seconds": 3, "python_version": "3.12.3"},
            {"system_setup_seconds": 8, "apt_updated": True, "missing_apt_packages": ["libmaxminddb-dev"], "pip_install_seconds": 5, "python_version": "3.12.3"},
            {"system_setup_seconds": 4, "apt_updated": False, "missing_apt_packages": [], "pip_install_seconds": 4, "python_version": "3.12.3"},
        ]
        summary = aggregate_records(
            [record("prepare-dependencies", setup=setup[0])]
            + [record(module, setup=item) for module, item in zip(modules, setup)],
            modules,
        )

        self.assertEqual(summary["setup"]["records_with_setup"], 4)
        self.assertEqual(summary["setup"]["system_setup_seconds"], {"min": 2, "max": 8, "average": 4})
        self.assertEqual(summary["setup"]["pip_install_seconds"], {"min": 3, "max": 5, "average": 3.75})
        self.assertEqual(summary["setup"]["apt_updated_jobs"], 1)
        self.assertEqual(summary["setup"]["missing_apt_packages"], ["libmaxminddb-dev"])

        markdown = render_markdown(summary)
        self.assertIn("System setup: `2–8s (avg 4s)`", markdown)
        self.assertIn("apt updates: `1/4`", markdown)
        self.assertIn("Missing apt packages: `libmaxminddb-dev`", markdown)

    def test_old_records_without_setup_are_unavailable(self):
        summary = aggregate_records(
            [record("prepare-dependencies"), record("VIP_A")],
            ["VIP_A"],
        )

        self.assertEqual(summary["setup"]["records_with_setup"], 0)
        self.assertEqual(
            summary["setup"]["system_setup_seconds"],
            {"min": None, "max": None, "average": None},
        )
        markdown = render_markdown(summary)
        self.assertIn("System setup: `unavailable`", markdown)
        self.assertIn("pip install: `unavailable`", markdown)
        self.assertIn("Missing apt packages: `unavailable`", markdown)

    def test_zero_setup_values_and_no_apt_update_are_preserved(self):
        zero_setup = {
            "system_setup_seconds": 0,
            "apt_updated": False,
            "missing_apt_packages": [],
            "pip_install_seconds": 0,
            "python_version": "3.12.3",
        }
        summary = aggregate_records(
            [record("prepare-dependencies", setup=zero_setup), record("VIP_A", setup=zero_setup)],
            ["VIP_A"],
        )

        self.assertEqual(summary["setup"]["system_setup_seconds"], {"min": 0, "max": 0, "average": 0})
        self.assertEqual(summary["setup"]["pip_install_seconds"], {"min": 0, "max": 0, "average": 0})
        self.assertEqual(summary["setup"]["apt_updated_jobs"], 0)
        self.assertEqual(summary["setup"]["missing_apt_packages"], [])


if __name__ == "__main__":
    unittest.main()
