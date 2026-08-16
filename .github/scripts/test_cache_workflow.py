"""Static contract tests for dependency cache behavior in build.yml."""

from __future__ import annotations

import hashlib
import re
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
WORKFLOW = ROOT / ".github" / "workflows" / "build.yml"
REQUIREMENTS = ROOT / ".github" / "ci-requirements.txt"
PREPARE_BUILD_TOOLS = ROOT / ".github" / "actions" / "prepare-build-tools" / "action.yml"

SDK_REFS = ("AMBUILD_REF", "MMS_REF", "HL2SDK_REF", "SCHEMAENTITY_REF", "MANIFEST_REF")


def sdk_cache_key(refs: dict[str, str], runner_arch: str = "X64", version: str = "v2") -> str:
    return "-".join(
        [
            version,
            "Linux",
            runner_arch,
            "ubuntu24.04",
            *(refs[name] for name in SDK_REFS),
        ]
    )


def pip_cache_key(requirements: str, runner_arch: str = "X64", version: str = "v2") -> str:
    requirements_hash = hashlib.sha256(requirements.encode("utf-8")).hexdigest()
    return f"pip-{version}-Linux-{runner_arch}-ubuntu24.04-py312-{requirements_hash}"


def cache_match_type(value: str | None) -> str:
    if value == "true":
        return "exact"
    if value == "false":
        return "partial"
    return "miss"


class CacheWorkflowTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.workflow = WORKFLOW.read_text(encoding="utf-8")
        cls.prepare_build_tools = PREPARE_BUILD_TOOLS.read_text(encoding="utf-8")

    def test_prepare_build_tools_action_is_pinned_and_standardizes_python(self):
        self.assertIn(
            "actions/setup-python@5fda3b95a4ea91299a34e894583c3862153e4b97",
            self.prepare_build_tools,
        )
        self.assertIn("python-version: '3.12'", self.prepare_build_tools)
        self.assertIn("check-latest: false", self.prepare_build_tools)
        self.assertNotIn("cache:", self.prepare_build_tools)

    def test_prepare_build_tools_is_used_by_both_build_jobs(self):
        self.assertEqual(self.workflow.count("uses: ./.github/actions/prepare-build-tools"), 2)
        self.assertEqual(self.workflow.count("id: prepare-build-tools"), 2)

    def test_system_packages_are_audited_before_conditional_apt_update(self):
        for package in (
            "binutils",
            "clang-18",
            "file",
            "git",
            "libmaxminddb-dev",
            "unzip",
            "zip",
        ):
            self.assertIn(f"          {package}", self.prepare_build_tools)
        self.assertIn("dpkg-query", self.prepare_build_tools)
        self.assertIn("if ((${#missing_packages[@]} > 0)); then", self.prepare_build_tools)
        self.assertEqual(self.prepare_build_tools.count("sudo apt-get update -qy"), 1)
        build_jobs = self.workflow.split("  package-release:", 1)[0]
        self.assertNotIn("sudo apt-get update -qy", build_jobs)
        self.assertIn("for command in clang-18 clang++-18 git file zip unzip", self.prepare_build_tools)

    def test_python_and_setup_telemetry_are_used_after_setup(self):
        self.assertNotIn("python3 -m pip", self.workflow)
        build_module = self.workflow.split("  build-module:", 1)[1].split("  package-release:", 1)[0]
        self.assertNotIn("python3", build_module)
        self.assertIn("python ../configure.py", self.workflow)
        self.assertEqual(self.workflow.count("python .github/scripts/cache_metrics.py record"), 2)
        for output in (
            "setup-seconds",
            "apt-updated",
            "missing-apt-packages",
            "pip_install_seconds",
            "python-version",
        ):
            self.assertIn(output, self.workflow)

    def test_cache_actions_are_split_between_restore_and_save(self):
        self.assertEqual(self.workflow.count("uses: actions/cache/restore@"), 4)
        self.assertEqual(self.workflow.count("uses: actions/cache/save@"), 2)
        self.assertNotIn("uses: actions/cache@", self.workflow)

    def test_cache_version_and_runner_arch_are_part_of_cache_keys(self):
        self.assertIn("CACHE_VERSION: v2", self.workflow)
        self.assertGreaterEqual(self.workflow.count("${{ runner.arch }}"), 8)

    def test_sdk_key_contains_all_pinned_dependency_refs(self):
        key_lines = [
            line for line in self.workflow.splitlines() if line.strip().startswith("key:")
        ]
        sdk_lines = [line for line in key_lines if "MANIFEST_REF" in line]
        self.assertEqual(len(sdk_lines), 3)
        for name in SDK_REFS:
            self.assertTrue(all(name in line for line in sdk_lines), name)

    def test_sdk_key_changes_when_any_pinned_ref_changes(self):
        refs = {name: name.lower() for name in SDK_REFS}
        original = sdk_cache_key(refs, runner_arch="X64")
        changed = dict(refs)
        changed["HL2SDK_REF"] = "changed-ref"
        self.assertNotEqual(original, sdk_cache_key(changed, runner_arch="X64"))

    def test_pip_key_changes_when_requirements_change(self):
        requirements = REQUIREMENTS.read_text(encoding="utf-8")
        original = pip_cache_key(requirements, runner_arch="X64")
        changed = pip_cache_key(requirements + "\n# dependency bump\n", runner_arch="X64")
        self.assertNotEqual(original, changed)
        self.assertIn("hashFiles('.github/ci-requirements.txt')", self.workflow)
        self.assertNotIn("hashFiles('.github/workflows/build.yml')", self.workflow)

    def test_ci_requirements_are_pinned(self):
        self.assertEqual(
            REQUIREMENTS.read_text(encoding="utf-8").splitlines(),
            ["pip==26.2.1", "importlib-metadata==9.0.0", "setuptools==45.2.0"],
        )

    def test_restore_status_contract_distinguishes_exact_partial_and_miss(self):
        self.assertEqual(cache_match_type("true"), "exact")
        self.assertEqual(cache_match_type("false"), "partial")
        self.assertEqual(cache_match_type(""), "miss")
        self.assertEqual(self.workflow.count("$(cache_match_type"), 4)
        self.assertIn("cache-matched-key", self.workflow)
        self.assertIn("cache-primary-key", self.workflow)

    def test_restore_and_save_failures_are_observable(self):
        self.assertGreaterEqual(self.workflow.count("continue-on-error: true"), 6)
        self.assertIn("steps.restore-dependencies.outcome", self.workflow)
        self.assertIn("steps.restore-pip.outcome", self.workflow)
        self.assertIn("steps.save-dependencies.outcome", self.workflow)
        self.assertIn("steps.save-pip.outcome", self.workflow)
        self.assertIn("CACHE_RESTORE_STARTED_AT", self.workflow)
        self.assertIn("CACHE_RESTORE_FINISHED_AT", self.workflow)
        self.assertIn("Restore window (SDK + pip)", self.workflow)

    def test_pull_requests_never_save_dependency_caches(self):
        save_blocks = re.findall(
            r"      - name: Save .*?\n(?P<body>.*?)(?=\n      - name:|\n  build-module:)",
            self.workflow,
            flags=re.DOTALL,
        )
        self.assertEqual(len(save_blocks), 2)
        for block in save_blocks:
            self.assertIn("github.event_name != 'pull_request'", block)
            self.assertIn("continue-on-error: true", block)

        self.assertIn("if [[ \"$DEPENDENCY_CACHE_HIT\" != \"true\" ]]", self.workflow)
        self.assertIn("Cache writes from pull requests: disabled", self.workflow)

    def test_fallback_is_kept_when_sdk_restore_misses(self):
        self.assertIn("if [[ \"$DEPENDENCY_CACHE_HIT\" != \"true\" ]]", self.workflow)
        self.assertIn('echo "fallback-used=true" >> "$GITHUB_OUTPUT"', self.workflow)
        self.assertIn('echo "fallback-used=false" >> "$GITHUB_OUTPUT"', self.workflow)
        self.assertIn("Fallback clone used:", self.workflow)

    def test_summary_uses_shell_variables_instead_of_escaped_literals(self):
        self.assertNotIn(r"\${DEPENDENCY_CACHE_HIT", self.workflow)
        self.assertNotIn(r"\${PIP_CACHE_HIT", self.workflow)
        self.assertNotIn(r"\${FALLBACK_USED", self.workflow)
        self.assertIn("SDK matched key:", self.workflow)
        self.assertIn("pip matched key:", self.workflow)

    def test_cache_telemetry_is_uploaded_per_job_and_aggregated(self):
        self.assertIn("cache_metrics.py record", self.workflow)
        self.assertIn("name: cache-metrics-${{ matrix.module }}", self.workflow)
        self.assertIn("pattern: cache-metrics-*", self.workflow)
        self.assertIn("cache_metrics.py aggregate", self.workflow)
        self.assertIn("name: cache-metrics-summary", self.workflow)
        telemetry_uploads = re.findall(
            r"- name: (?:Upload dependency cache telemetry|Upload module cache telemetry|Upload aggregate cache telemetry)\n(.*?)(?=\n      - name:|\Z)",
            self.workflow,
            flags=re.DOTALL,
        )
        self.assertEqual(len(telemetry_uploads), 3)
        self.assertTrue(all("archive: true" in block for block in telemetry_uploads))


if __name__ == "__main__":
    unittest.main()
