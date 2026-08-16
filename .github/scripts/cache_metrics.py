"""Write and aggregate GitHub Actions cache telemetry records."""

from __future__ import annotations

import argparse
import json
import os
import sys
from pathlib import Path
from statistics import mean
from typing import Iterable, Mapping


SCHEMA_VERSION = 1
PREPARE_JOB = "prepare-dependencies"


def _value(env: Mapping[str, str], name: str) -> str | None:
    value = env.get(name, "").strip()
    return value or None


def _seconds(env: Mapping[str, str]) -> int | None:
    started = _value(env, "CACHE_RESTORE_STARTED_AT")
    finished = _value(env, "CACHE_RESTORE_FINISHED_AT")
    if not started or not finished or not started.isdigit() or not finished.isdigit():
        return None
    result = int(finished) - int(started)
    return result if result >= 0 else None


def _integer(env: Mapping[str, str], name: str) -> int | None:
    value = _value(env, name)
    if value is None or not value.isdigit():
        return None
    return int(value)


def _boolean(env: Mapping[str, str], name: str) -> bool | None:
    value = _value(env, name)
    if value is None:
        return None
    if value.lower() == "true":
        return True
    if value.lower() == "false":
        return False
    return None


def _packages(env: Mapping[str, str], name: str) -> list[str]:
    value = _value(env, name)
    return value.split() if value else []


def record_from_environment(job: str, env: Mapping[str, str] | None = None) -> dict:
    """Build one JSON-safe telemetry record from GitHub's environment variables."""

    env = env or os.environ
    return {
        "schema_version": SCHEMA_VERSION,
        "run_id": _value(env, "GITHUB_RUN_ID"),
        "commit": _value(env, "GITHUB_SHA"),
        "repository": _value(env, "GITHUB_REPOSITORY"),
        "event": _value(env, "GITHUB_EVENT_NAME"),
        "job": job,
        "runner": {
            "os": _value(env, "RUNNER_OS"),
            "arch": _value(env, "RUNNER_ARCH"),
        },
        "sdk": {
            "primary_key": _value(env, "SDK_CACHE_PRIMARY_KEY"),
            "matched_key": _value(env, "SDK_CACHE_MATCHED_KEY"),
            "hit": _value(env, "DEPENDENCY_CACHE_HIT"),
            "restore_outcome": _value(env, "DEPENDENCY_RESTORE_OUTCOME"),
            "save_outcome": _value(env, "SDK_CACHE_SAVE_OUTCOME"),
            "fallback_used": _value(env, "FALLBACK_USED"),
        },
        "pip": {
            "primary_key": _value(env, "PIP_CACHE_PRIMARY_KEY"),
            "matched_key": _value(env, "PIP_CACHE_MATCHED_KEY"),
            "hit": _value(env, "PIP_CACHE_HIT"),
            "restore_outcome": _value(env, "PIP_RESTORE_OUTCOME"),
            "save_outcome": _value(env, "PIP_CACHE_SAVE_OUTCOME"),
        },
        "restore_seconds": _seconds(env),
        "setup": {
            "system_setup_seconds": _integer(env, "SYSTEM_SETUP_SECONDS"),
            "apt_updated": _boolean(env, "APT_UPDATED"),
            "missing_apt_packages": _packages(env, "MISSING_APT_PACKAGES"),
            "pip_install_seconds": _integer(env, "PIP_INSTALL_SECONDS"),
            "python_version": _value(env, "PYTHON_VERSION"),
        },
    }


def _hit_bucket(value: str | None) -> str:
    if value == "true":
        return "exact"
    if value == "false":
        return "partial"
    return "miss"


def _hit_counts(records: Iterable[dict], cache_name: str) -> dict[str, int]:
    counts = {"exact": 0, "partial": 0, "miss": 0}
    for record in records:
        counts[_hit_bucket(record.get(cache_name, {}).get("hit"))] += 1
    return counts


def _stats(values: Iterable[int]) -> dict[str, int | float | None]:
    values = list(values)
    return {
        "min": min(values) if values else None,
        "max": max(values) if values else None,
        "average": round(mean(values), 2) if values else None,
    }


def _setup_stats(records: Iterable[dict]) -> dict:
    setup_records = [
        record.get("setup")
        for record in records
        if isinstance(record.get("setup"), Mapping)
    ]
    system_values = [
        setup["system_setup_seconds"]
        for setup in setup_records
        if isinstance(setup.get("system_setup_seconds"), int)
        and not isinstance(setup.get("system_setup_seconds"), bool)
    ]
    pip_values = [
        setup["pip_install_seconds"]
        for setup in setup_records
        if isinstance(setup.get("pip_install_seconds"), int)
        and not isinstance(setup.get("pip_install_seconds"), bool)
    ]
    missing_packages = sorted(
        {
            package
            for setup in setup_records
            for package in setup.get("missing_apt_packages", [])
            if isinstance(package, str) and package
        }
    )
    return {
        "records_with_setup": len(setup_records),
        "system_setup_seconds": _stats(system_values),
        "pip_install_seconds": _stats(pip_values),
        "apt_updated_jobs": sum(setup.get("apt_updated") is True for setup in setup_records),
        "apt_updated_total": len(records),
        "missing_apt_packages": missing_packages,
    }


def aggregate_records(records: Iterable[dict], expected_modules: Iterable[str]) -> dict:
    """Aggregate records and return warnings instead of failing the workflow."""

    records = list(records)
    expected = set(expected_modules)
    module_records = [record for record in records if record.get("job") != PREPARE_JOB]
    module_names = [str(record.get("job")) for record in module_records]
    module_name_set = set(module_names)
    duplicate_modules = sorted(
        name for name in module_name_set if module_names.count(name) > 1
    )
    missing_modules = sorted(expected - module_name_set)
    unexpected_modules = sorted(module_name_set - expected)
    warnings: list[str] = []

    if missing_modules:
        warnings.append(f"missing module telemetry: {', '.join(missing_modules)}")
    if unexpected_modules:
        warnings.append(f"unexpected module telemetry: {', '.join(unexpected_modules)}")
    if duplicate_modules:
        warnings.append(f"duplicate module telemetry: {', '.join(duplicate_modules)}")
    if not any(record.get("job") == PREPARE_JOB for record in records):
        warnings.append("missing prepare-dependencies telemetry")

    sdk = _hit_counts(module_records, "sdk")
    pip = _hit_counts(module_records, "pip")
    fallback_count = sum(
        record.get("sdk", {}).get("fallback_used") == "true" for record in module_records
    )
    restore_values = [
        record["restore_seconds"]
        for record in module_records
        if isinstance(record.get("restore_seconds"), int)
    ]

    for record in module_records:
        job = record.get("job", "unknown")
        if record.get("sdk", {}).get("hit") != "true":
            warnings.append(f"SDK cache was not exact for {job}")
        if record.get("sdk", {}).get("fallback_used") == "true":
            warnings.append(f"SDK fallback clone was used for {job}")
        if record.get("pip", {}).get("hit") != "true":
            warnings.append(f"pip cache was not exact for {job}")

    summary = {
        "schema_version": SCHEMA_VERSION,
        "run_id": next((record.get("run_id") for record in records if record.get("run_id")), None),
        "commit": next((record.get("commit") for record in records if record.get("commit")), None),
        "expected_module_count": len(expected),
        "actual_module_count": len(module_name_set),
        "records_seen": len(records),
        "missing_modules": missing_modules,
        "unexpected_modules": unexpected_modules,
        "duplicate_modules": duplicate_modules,
        "sdk": {
            **sdk,
            "total": sum(sdk.values()),
            "exact_hit_rate": round(sdk["exact"] / len(module_records), 4)
            if module_records
            else 0.0,
        },
        "pip": {
            **pip,
            "total": sum(pip.values()),
            "exact_hit_rate": round(pip["exact"] / len(module_records), 4)
            if module_records
            else 0.0,
        },
        "fallback_count": fallback_count,
        "restore_seconds": {
            **_stats(restore_values),
        },
        "setup": _setup_stats(records),
        "warnings": warnings,
    }
    return summary


def aggregate_directory(input_dir: Path, expected_modules: Iterable[str]) -> dict:
    records: list[dict] = []
    malformed: list[str] = []
    for path in sorted(input_dir.glob("*.json")) if input_dir.is_dir() else []:
        try:
            record = json.loads(path.read_text(encoding="utf-8"))
            if not isinstance(record, dict) or record.get("schema_version") != SCHEMA_VERSION:
                raise ValueError("unsupported telemetry schema")
            records.append(record)
        except (OSError, json.JSONDecodeError, TypeError, ValueError):
            malformed.append(path.name)

    summary = aggregate_records(records, expected_modules)
    summary["malformed_files"] = malformed
    if malformed:
        summary["warnings"].append(f"malformed telemetry files: {', '.join(malformed)}")
    return summary


def render_markdown(summary: dict) -> str:
    sdk = summary["sdk"]
    pip = summary["pip"]
    restore = summary["restore_seconds"]
    setup = summary.get("setup", {})
    restore_range = (
        f"{restore['min']}–{restore['max']}s (avg {restore['average']}s)"
        if restore["min"] is not None
        else "unavailable"
    )
    system_setup = setup.get("system_setup_seconds", {})
    pip_install = setup.get("pip_install_seconds", {})
    system_setup_range = (
        f"{system_setup['min']}–{system_setup['max']}s (avg {system_setup['average']}s)"
        if system_setup.get("min") is not None
        else "unavailable"
    )
    pip_install_range = (
        f"{pip_install['min']}–{pip_install['max']}s (avg {pip_install['average']}s)"
        if pip_install.get("min") is not None
        else "unavailable"
    )
    setup_records = setup.get("records_with_setup", 0)
    apt_updates = setup.get("apt_updated_jobs", 0)
    missing_packages = setup.get("missing_apt_packages", [])
    setup_status = "unavailable" if not setup_records else f"{apt_updates}/{setup.get('apt_updated_total', 0)}"
    missing_status = ", ".join(missing_packages) if setup_records else "unavailable"
    lines = [
        "### Cache telemetry",
        f"- Module records: `{summary['actual_module_count']}/{summary['expected_module_count']}`",
        f"- SDK exact hits: `{sdk['exact']}/{sdk['total']}` ({sdk['exact_hit_rate']:.0%})",
        f"- pip matches: exact=`{pip['exact']}`, partial=`{pip['partial']}`, miss=`{pip['miss']}`",
        f"- SDK fallback clones: `{summary['fallback_count']}`",
        f"- Restore window: `{restore_range}`",
        f"- System setup: `{system_setup_range}`",
        f"- pip install: `{pip_install_range}`",
        f"- apt updates: `{setup_status}`",
        f"- Missing apt packages: `{missing_status}`",
    ]
    warnings = summary.get("warnings", [])
    if warnings:
        lines.append(f"- Warnings: `{len(warnings)}`")
        lines.extend(f"  - {warning}" for warning in warnings)
    else:
        lines.append("- Warnings: none")
    return "\n".join(lines) + "\n"


def _expected_modules_from_environment(env: Mapping[str, str]) -> list[str]:
    raw = env.get("EXPECTED_MODULES_JSON", "[]")
    try:
        payload = json.loads(raw)
    except json.JSONDecodeError:
        return []
    if not isinstance(payload, list):
        return []
    return [item["module"] if isinstance(item, dict) else str(item) for item in payload]


def _record_command(args: argparse.Namespace) -> int:
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    record = record_from_environment(args.job)
    output.write_text(json.dumps(record, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return 0


def _aggregate_command(args: argparse.Namespace) -> int:
    summary = aggregate_directory(Path(args.input_dir), _expected_modules_from_environment(os.environ))
    Path(args.output_json).write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    markdown = render_markdown(summary)
    if args.summary_file:
        with Path(args.summary_file).open("a", encoding="utf-8") as handle:
            handle.write(markdown)
    print(markdown, end="")
    for warning in summary.get("warnings", []):
        print(f"::warning title=Cache telemetry::{warning}")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    record_parser = subparsers.add_parser("record")
    record_parser.add_argument("--job", required=True)
    record_parser.add_argument("--output", required=True)
    record_parser.set_defaults(handler=_record_command)

    aggregate_parser = subparsers.add_parser("aggregate")
    aggregate_parser.add_argument("--input-dir", default="cache-metrics")
    aggregate_parser.add_argument("--output-json", default="cache-metrics-summary.json")
    aggregate_parser.add_argument("--summary-file")
    aggregate_parser.set_defaults(handler=_aggregate_command)

    args = parser.parse_args(argv)
    return args.handler(args)


if __name__ == "__main__":
    sys.exit(main())
