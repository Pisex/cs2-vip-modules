"""Static contract tests for the automated GitHub Release workflow."""

from pathlib import Path
import unittest


WORKFLOW = Path(__file__).resolve().parents[1] / "workflows" / "build.yml"


class ReleaseWorkflowContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.workflow = WORKFLOW.read_text(encoding="utf-8")
        marker = "  create-release:\n"
        if marker not in cls.workflow:
            raise AssertionError("create-release job is missing")
        cls.release_job = cls.workflow.split(marker, 1)[1]

    def test_push_and_pull_request_branches(self):
        self.assertIn(
            "push:\n    branches:\n      - main\n      - dev",
            self.workflow,
        )
        self.assertIn(
            "pull_request:\n    branches:\n      - main",
            self.workflow,
        )
        self.assertNotIn("branches: [dev]", self.workflow)

    def test_release_runs_only_after_successful_main_push(self):
        self.assertIn("needs: [discover, build-summary]", self.release_job)
        self.assertIn("github.event_name == 'push'", self.release_job)
        self.assertIn("github.ref == 'refs/heads/main'", self.release_job)
        self.assertIn("needs.discover.result == 'success'", self.release_job)
        self.assertIn("needs.build-summary.result == 'success'", self.release_job)
        self.assertNotIn("github.event_name == 'pull_request'", self.release_job)
        self.assertNotIn("github.ref == 'refs/heads/dev'", self.release_job)
        self.assertNotIn("workflow_dispatch", self.release_job)

    def test_release_permissions_and_serialization(self):
        self.assertEqual(self.workflow.count("contents: write"), 1)
        self.assertIn("permissions:\n      contents: write", self.release_job)
        self.assertIn("GH_TOKEN: ${{ github.token }}", self.release_job)
        self.assertIn("concurrency:", self.release_job)
        self.assertIn("group: release-1.0-bywinsty", self.release_job)
        self.assertIn("cancel-in-progress: false", self.release_job)

    def test_tag_release_and_main_tip_guard(self):
        self.assertIn("RELEASE_TAG: 1.0-bywinsty", self.release_job)
        self.assertIn("RELEASE_TITLE: 1.0-bywinsty", self.release_job)
        self.assertIn("git/ref/heads/main", self.release_job)
        self.assertIn("refs/tags/$RELEASE_TAG", self.release_job)
        self.assertIn("force=true", self.release_job)
        self.assertIn('gh release delete "$RELEASE_TAG"', self.release_job)
        self.assertNotIn("--cleanup-tag", self.release_job)
        self.assertIn("gh release create", self.release_job)
        self.assertIn("--draft", self.release_job)
        self.assertIn("gh release edit", self.release_job)
        self.assertIn("--draft=false", self.release_job)

    def test_release_notes_follow_current_commit(self):
        self.assertIn("Automated release from main.", self.release_job)
        self.assertIn("$GITHUB_SHA", self.release_job)
        self.assertIn("$GITHUB_RUN_ID", self.release_job)

    def test_release_contains_exact_expected_assets(self):
        self.assertIn("actions/download-artifact@", self.release_job)
        self.assertIn("pattern: VIP_*.tar.gz", self.release_job)
        self.assertIn('expected.add("VIP_All_Modules.tar.gz")', self.release_job)
        self.assertIn("if len(actual) != 36", self.release_job)
        self.assertIn("35 module archives and VIP_All_Modules.tar.gz", self.release_job)
        self.assertIn("gh release upload", self.release_job)
        self.assertIn("--clobber", self.release_job)
        self.assertIn("Verify release assets", self.release_job)
        self.assertIn('test "${#actual[@]}" -eq 36', self.release_job)

    def test_combined_archive_contains_nested_modules_archive(self):
        package_job = self.workflow.split("  package-release:", 1)[1].split("  build-summary:", 1)[0]
        self.assertIn(
            'tar -czf "$GITHUB_WORKSPACE/release/Modules.tar.gz" VIP_*.tar.gz',
            package_job,
        )
        self.assertIn("cp release/Modules.tar.gz combined-root/Modules.tar.gz", package_job)
        self.assertIn(
            "tar -czf release/VIP_All_Modules.tar.gz -C combined-root addons Modules.tar.gz",
            package_job,
        )
        self.assertIn("expected_module_archives", package_job)
        self.assertIn(
            'validate_members(modules_archive, expected_module_archives, set(), "Modules.tar.gz")',
            package_job,
        )
        self.assertNotIn("path: release/Modules.tar.gz", package_job)

    def test_build_info_is_never_published(self):
        self.assertIn("Modules_Build_Info.zip", self.release_job)
        self.assertNotIn(
            'gh release upload "$RELEASE_TAG" "release-assets/Modules_Build_Info.zip"',
            self.release_job,
        )
        self.assertNotIn("Source code", self.release_job)


if __name__ == "__main__":
    unittest.main()
