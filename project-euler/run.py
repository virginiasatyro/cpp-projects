import os
import sys
import json
import subprocess
import time
import tempfile

COMPILER = "g++"
FLAGS = ["-std=c++20", "-O2"]

WRAPPER_TEMPLATE = r"""
#include "{solution}"
#include <iostream>
#include <vector>

int main() {{
    int n, target;
    std::cin >> n;

    std::vector<int> nums(n);
    for (int i = 0; i < n; i++) std::cin >> nums[i];
    std::cin >> target;

    auto out = {classname}().run(nums, target);

    for (auto &x : out) std::cout << x << " ";
    return 0;
}}
"""

def compile_with_wrapper(src, exe):
    # Caminho absoluto para o arquivo .cpp
    abs_src = os.path.abspath(src)

    classname = os.path.splitext(os.path.basename(src))[0].capitalize()

    wrapper_code = WRAPPER_TEMPLATE.format(
        solution=abs_src.replace("\\", "/"),   # funciona no Windows
        classname=classname
    )

    with tempfile.NamedTemporaryFile(delete=False, suffix=".cpp") as tmp:
        tmp.write(wrapper_code.encode("utf-8"))
        wrapper_path = tmp.name.replace("\\", "/")

    cmd = [COMPILER] + FLAGS + [wrapper_path, abs_src, "-o", exe]
    subprocess.run(cmd, check=True)

def run_solution(exe, nums, target):
    stdin = f"{len(nums)} " + " ".join(map(str, nums)) + f" {target}"
    result = subprocess.run(
        [exe],
        input=stdin,
        text=True,
        capture_output=True
    )
    out = result.stdout.strip().split()
    return list(map(int, out)) if out else []

def progress(i, total):
    pct = i / total * 100
    bar = int(pct // 2)
    print(f"[{'#'*bar}{'.'*(50-bar)}] {pct:5.1f}%", end='\r')

def run_problem(name):
    problem_dir = os.path.join("problems", name)

    with open(os.path.join(problem_dir, "tests.json")) as f:
        tests = json.load(f)["tests"]

    solutions = [
        f for f in os.listdir(problem_dir)
        if f.startswith("solution") and f.endswith(".cpp")
    ]

    print(f"\n=== Running {name} ===\n")

    results = []

    for sol in solutions:
        src = os.path.join(problem_dir, sol)
        exe = os.path.join(problem_dir, sol.replace(".cpp", ".exe"))

        print(f"\nCompiling {sol}...")
        compile_with_wrapper(src, exe)

        print(f"Testing {sol}...")
        start = time.perf_counter()

        passed = 0
        for i, t in enumerate(tests, start=1):
            progress(i, len(tests))
            out = run_solution(
                exe,
                t["input"]["nums"],
                t["input"]["target"]
            )
            if out == t["expected"]:
                passed += 1

        total = time.perf_counter() - start
        print()

        ok = (passed == len(tests))
        print(f"  Passed: {passed}/{len(tests)}  {'✔ PASS' if ok else '✘ FAIL'}")
        print(f"  Time: {total:.6f}s")

        results.append((sol, total, ok))

    print("\n=== Ranking ===")
    results.sort(key=lambda x: x[1])
    for rank, (sol, t, ok) in enumerate(results, 1):
        print(f"{rank}. {sol:<20} {t:.6f}s   ({'PASS' if ok else 'FAIL'})")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python run.py <problem_name>")
        sys.exit(1)
    run_problem(sys.argv[1])