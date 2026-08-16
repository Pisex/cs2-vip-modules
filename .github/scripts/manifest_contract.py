#!/usr/bin/env python3
"""Derive CI package invariants from the package manifest."""

import argparse
import json
import sys
from pathlib import Path


def load_contract(path):
    manifest = json.loads(path.read_text(encoding="utf-8"))
    packages = manifest.get("packages")
    if not isinstance(packages, dict) or not packages:
        raise ValueError("manifest must contain a non-empty packages object")

    package_files = {}
    all_files = []
    for module, package in sorted(packages.items()):
        files = package.get("files")
        binary = package.get("binary")
        vdf = package.get("vdf")
        if not isinstance(files, list) or not files:
            raise ValueError(f"{module}: files must be a non-empty list")
        if len(files) != len(set(files)):
            raise ValueError(f"{module}: duplicate package file paths")
        if binary not in files or vdf not in files:
            raise ValueError(f"{module}: binary and vdf must be listed in files")
        package_files[module] = tuple(files)
        all_files.extend(files)

    if len(all_files) != len(set(all_files)):
        duplicates = sorted({path for path in all_files if all_files.count(path) > 1})
        raise ValueError(f"manifest contains duplicate file paths: {duplicates}")

    module_archives = [f"{module}.tar.gz" for module in sorted(packages)]
    return {
        "module_count": len(packages),
        "package_file_count": len(set(all_files)),
        "module_archive_count": len(module_archives),
        "release_archive_count": len(module_archives) + 1,
        "report_count": len(packages),
        "module_archives": module_archives,
        "combined_archive": "VIP_All_Modules.tar.gz",
    }


def write_github_outputs(contract, output_path):
    with output_path.open("a", encoding="utf-8") as output:
        for key in (
            "module_count",
            "package_file_count",
            "module_archive_count",
            "release_archive_count",
            "report_count",
            "combined_archive",
        ):
            output.write(f"{key}={contract[key]}\n")
        output.write(
            "module_archives="
            + json.dumps(contract["module_archives"], separators=(",", ":"))
            + "\n"
        )


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--github-output", type=Path)
    args = parser.parse_args(argv)

    try:
        contract = load_contract(args.manifest)
        if args.github_output:
            write_github_outputs(contract, args.github_output)
        print(json.dumps(contract, sort_keys=True))
        return 0
    except (OSError, ValueError, json.JSONDecodeError) as exc:
        print(f"manifest contract failed: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
