"""Run both applications in isolated build folders, preserving working data."""

from pathlib import Path
import hashlib
import subprocess


ROOT = Path(__file__).resolve().parents[1]


def fingerprint(path):
    return hashlib.sha256(path.read_bytes()).hexdigest() if path.exists() else None


def run(config, project, name, text, filename):
    folder = ROOT / "build" / "scenarios" / config / project / name
    folder.mkdir(parents=True, exist_ok=True)
    target = folder / filename
    if text is not None:
        target.write_text(text, encoding="utf-8")
    elif target.exists():
        target.unlink()
    return subprocess.run(
        [str(ROOT / "build" / "x64" / config / project / f"{project}.exe")],
        cwd=folder, capture_output=True, text=True, encoding="utf-8", timeout=10,
    ), folder


def main():
    working = [ROOT / "lab_01" / "input.txt", ROOT / "lab_01" / "output.txt",
               ROOT / "homework" / "data.txt"]
    before = {path: fingerprint(path) for path in working}
    lab_cases = [
        ("empty", "", [], False),
        ("one", "3", [3], False),
        ("even", "8 2 6 4", [2, 4, 6, 8], False),
        ("isolated", "8 3 2 7 1 6 5 4 -3 -2",
         [-3, -2, 1, 2, 3, 4, 5, 6, 7, 8], False),
        ("front", "8 3 2 7 1 6 5 4 -3 -5 -2",
         [-2, 1, 2, 3, 4, 5, 6, 7, 8, -5, -3], True),
        ("middle", "8 7 2 1 5 4 3", [1, 2, 3, 4, 8, 5, 7], True),
        ("tail", "2 4 1 5 7", [1, 2, 4, 5, 7], True),
        ("all_odd", "5 3 1", [1, 3, 5], True),
        ("duplicates", "-3 -3 -2 0 1 1 2", [-2, 0, 1, 1, 2, -3, -3], True),
        ("several", "-5 -3 -2 1 3 4 5 7 8",
         [-2, 1, 3, 4, 5, 7, 8, -5, -3], True),
    ]
    count = 0
    for config in ("Debug", "Release"):
        for name, text, expected, found in lab_cases:
            result, folder = run(config, "lab_01", name, text, "input.txt")
            assert result.returncode == 0 and not result.stderr, (name, result)
            lines = (folder / "output.txt").read_text(encoding="utf-8").splitlines()
            values = list(map(int, lines[0].split())) if expected else []
            assert values == expected, (name, lines, expected)
            assert bool(len(lines) > 1) == (not found), (name, lines)
            if not expected:
                assert lines[0] == "Empty list"
            count += 1
        for project, filename in (("lab_01", "input.txt"), ("homework", "data.txt")):
            for name, text in (("missing", None), ("bad", "1 x"),
                               ("partial", "1 2x"), ("overflow", "2147483648")):
                result, _ = run(config, project, name, text, filename)
                assert result.returncode == 1 and "Error:" in result.stderr, (name, result)
                count += 1
        for name, text in (("example", "3 1 5 1 4 1"), ("empty", ""), ("only_five", "5")):
            result, _ = run(config, "homework", name, text, "data.txt")
            assert result.returncode == 0 and not result.stderr, (name, result)
            if name == "example":
                assert "Maximum: 5" in result.stdout
                assert "Reverse: 1 4 1 5 1 3" in result.stdout
                assert "3 2 2 2 4 4 2" in result.stdout
            elif name == "empty":
                assert "Empty list" in result.stdout and "Maximum:" not in result.stdout
            else:
                assert "Deleted: true" in result.stdout
                assert "Empty list" in result.stdout and "Duplicate first" not in result.stdout
            count += 1
    assert before == {path: fingerprint(path) for path in working}, "Working data changed"
    print(f"Passed {count} application scenarios; working input/output files preserved.")


if __name__ == "__main__":
    main()
