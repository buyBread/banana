import argparse
import subprocess
import os
import re
from pathlib import Path

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("executable", type=Path, help="path to the compiled test executable")

    args       = parser.parse_args()
    executable = args.executable.resolve()

    if not executable.is_file():
        parser.error(f"test executable not found: {executable}")

    project_dir = Path(__file__).resolve().parent.parent
    categories = sorted(directory.name
                            for directory in (project_dir / "tests").iterdir()
                                if directory.is_dir() and directory.name != "support")
    if not categories:
        parser.error("no test categories found")

    log_dir = project_dir / "build" / "test-results"
    log_dir.mkdir(parents=True, exist_ok=True)
    
    github_actions = os.environ.get("GITHUB_ACTIONS") == "true"
    
    failed = False

    for category in categories:
        print(f"::group::{category}" if github_actions else f"Running {category}", flush=True)
        
        try:
            result = subprocess.run(
                [ str(executable), f"--test-suite={category}", "--no-colors", "--no-breaks", "--order-by=name" ],
                cwd=project_dir,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=60,
                check=False)
            
            output = result.stdout
            exit_code = result.returncode
            error = f"{category} failed or selected no test cases; exit code {exit_code}"
        except subprocess.TimeoutExpired as exception:
            output = (exception.stdout or b"").decode("utf-8", errors="replace")
            exit_code = None
            error = f"{category} timed out after 60 seconds"
        except OSError as exception:
            output = f"{exception}\n"
            exit_code = None
            error = f"{category} could not run: {exception}"

        (log_dir / f"{category}.log").write_text(output, encoding="utf-8")
        print(output, end="" if output.endswith("\n") else "\n", flush=True)
        
        if github_actions:
            print("::endgroup::", flush=True)

        if exit_code != 0 or not re.search(r"\[doctest\] test cases:\s+[1-9]\d*\s+\|", output):
            print(f"::error::{error}" if github_actions else error, flush=True)
            failed = True

    return int(failed)

if __name__ == "__main__":
    raise SystemExit(main())
