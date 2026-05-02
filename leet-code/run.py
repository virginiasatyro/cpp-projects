import json
import os
import subprocess
import sys
import tempfile
import time

COMPILER = "g++"
FLAGS = ["-std=c++20", "-O2"]


WRAPPER_TEMPLATE = r"""
#include "{solution}"
#include <iostream>
#include <string>
#include <vector>

template <typename T>
void readValue(std::istream& in, T& value) {{
    in >> value;
}}

template <typename T>
void readValue(std::istream& in, std::vector<T>& values) {{
    int n = 0;
    in >> n;
    values.resize(n);
    for (int i = 0; i < n; i++) in >> values[i];
}}

template <typename T>
void printValue(const T& value) {{
    std::cout << value;
}}

template <typename T>
void printValue(const std::vector<T>& values) {{
    for (size_t i = 0; i < values.size(); i++) {{
        if (i > 0) std::cout << " ";
        std::cout << values[i];
    }}
}}

int main() {{
{declarations}

{reads}

    auto out = {classname}().run({arguments});
    printValue(out);
    return 0;
}}
"""


def class_name_from_source(src):
    name = os.path.splitext(os.path.basename(src))[0]
    return "".join(part[:1].upper() + part[1:] for part in name.split("_") if part)


def normalize_input(test_input, input_order=None):
    if isinstance(test_input, list):
        return test_input

    if isinstance(test_input, dict):
        if input_order:
            return [test_input[name] for name in input_order]
        return list(test_input.values())

    return [test_input]


def infer_cpp_type(value):
    if isinstance(value, bool):
        return "bool"
    if isinstance(value, int):
        return "int"
    if isinstance(value, float):
        return "double"
    if isinstance(value, str):
        return "std::string"
    if isinstance(value, list):
        if not value:
            return "std::vector<int>"
        return f"std::vector<{infer_cpp_type(value[0])}>"

    raise TypeError(f"Unsupported input type in tests.json: {type(value).__name__}")


def serialize_value(value):
    if isinstance(value, list):
        return " ".join([str(len(value))] + [str(v) for v in value])
    return str(value)


def parse_output(stdout, expected):
    text = stdout.strip()

    if isinstance(expected, list):
        if not text:
            return []
        if not expected:
            return text.split()
        if isinstance(expected[0], bool):
            return [value in ("1", "true", "True") for value in text.split()]
        if isinstance(expected[0], int):
            return list(map(int, text.split()))
        if isinstance(expected[0], float):
            return list(map(float, text.split()))
        return text.split()

    if isinstance(expected, bool):
        return text in ("1", "true", "True")
    if isinstance(expected, int):
        return int(text) if text else None
    if isinstance(expected, float):
        return float(text) if text else None

    return text


def compile_with_wrapper(src, exe, sample_args):
    abs_src = os.path.abspath(src).replace("\\", "/")
    classname = class_name_from_source(src)

    arg_types = [infer_cpp_type(value) for value in sample_args]
    declarations = "\n".join(f"    {arg_type} arg{i};" for i, arg_type in enumerate(arg_types))
    reads = "\n".join(f"    readValue(std::cin, arg{i});" for i in range(len(arg_types)))
    arguments = ", ".join(f"arg{i}" for i in range(len(arg_types)))

    wrapper_code = WRAPPER_TEMPLATE.format(
        solution=abs_src,
        classname=classname,
        declarations=declarations,
        reads=reads,
        arguments=arguments,
    )

    with tempfile.NamedTemporaryFile(delete=False, suffix=".cpp") as tmp:
        tmp.write(wrapper_code.encode("utf-8"))
        wrapper_path = tmp.name.replace("\\", "/")

    cmd = [COMPILER] + FLAGS + [wrapper_path, "-o", exe]
    subprocess.run(cmd, check=True)


def run_solution(exe, args, expected):
    stdin = " ".join(serialize_value(value) for value in args)
    result = subprocess.run([exe], input=stdin, text=True, capture_output=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip())
    return parse_output(result.stdout, expected)


def progress(i, total):
    pct = i / total * 100
    bar = int(pct // 2)
    print(f"[{'#' * bar}{'.' * (50 - bar)}] {pct:5.1f}%", end="\r")


def find_problem_dir(name):
    candidates = [
        name,
        os.path.join("problems", name),
    ]

    for candidate in candidates:
        if os.path.isdir(candidate) and os.path.exists(os.path.join(candidate, "tests.json")):
            return candidate

    raise FileNotFoundError(
        f"Could not find tests.json for problem '{name}'. Tried: "
        + ", ".join(os.path.join(candidate, "tests.json") for candidate in candidates)
    )


def run_problem(name):
    problem_dir = find_problem_dir(name)

    with open(os.path.join(problem_dir, "tests.json"), encoding="utf-8") as f:
        data = json.load(f)

    tests = data["tests"]
    input_order = data.get("input_order")
    sample_args = normalize_input(tests[0]["input"], input_order)

    solutions = [
        f for f in os.listdir(problem_dir) if f.startswith("solution") and f.endswith(".cpp")
    ]

    print(f"\n=== Running {name} ===\n")

    results = []

    for sol in solutions:
        src = os.path.join(problem_dir, sol)
        exe = os.path.join(problem_dir, sol.replace(".cpp", ".exe"))

        print(f"\nCompiling {sol}...")
        compile_with_wrapper(src, exe, sample_args)

        print(f"Testing {sol}...")
        start = time.perf_counter()

        passed = 0
        for i, test in enumerate(tests, start=1):
            progress(i, len(tests))
            args = normalize_input(test["input"], input_order)
            out = run_solution(exe, args, test["expected"])
            if out == test["expected"]:
                passed += 1
            else:
                print(
                    f"\n  Test {i} failed: input={test['input']} "
                    f"expected={test['expected']} got={out}"
                )

        total = time.perf_counter() - start
        print()

        ok = passed == len(tests)
        print(f"  Passed: {passed}/{len(tests)}  {'PASS' if ok else 'FAIL'}")
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
