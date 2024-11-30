import os
import subprocess


def find_test_files(directory):
    """
    查找指定目录中的所有 .in 和 .out 文件，并进行同名匹配。
    
    参数:
    - directory: 包含测试文件的目录路径
    
    返回:
    - 文件名匹配的测试用例列表 [(in_file, out_file), ...]
    """
    in_files = {}
    out_files = {}
    
    # 遍历目录下的文件，按扩展名分类
    for filename in os.listdir(directory):
        if filename.endswith(".in"):
            test_name = os.path.splitext(filename)[0]
            in_files[test_name] = os.path.join(directory, filename)
        elif filename.endswith(".out"):
            test_name = os.path.splitext(filename)[0]
            out_files[test_name] = os.path.join(directory, filename)
    
    # 匹配同名的 .in 和 .out 文件
    test_cases = []
    for test_name in in_files:
        if test_name in out_files:
            test_cases.append((in_files[test_name], out_files[test_name]))
    
    return test_cases


def normalize_double(value):
    """
    将浮点数字符串标准化，移除小数点后多余的零。
    """
    try:
        return str(float(value))  # 将字符串转换为浮点数再转换为字符串
    except ValueError:
        return value  # 如果不是数字，返回原值


def compare_output(command, actual_output, expected_output):
    """
    比较程序的输出和预期输出，根据命令不同调整比较规则。
    """
    if command in ["PRINT", "FINDALL"]:
        # 忽略顺序，比较内容
        actual_set = set(actual_output.split())
        expected_set = set(expected_output.split())
        return actual_set == expected_set
    elif command == "HIGHEST":
        # 忽略前两个ID顺序，比较权重
        actual_parts = actual_output.split()
        expected_parts = expected_output.split()

        if len(actual_parts) != 3 or len(expected_parts) != 3:
            return False

        actual_ids = set(actual_parts[:2])
        expected_ids = set(expected_parts[:2])
        actual_weight = normalize_double(actual_parts[2])
        expected_weight = normalize_double(expected_parts[2])

        return actual_ids == expected_ids and actual_weight == expected_weight
    elif command == "PATH":
        # 比较完整路径，最后一个权重需要标准化
        actual_parts = actual_output.split()
        expected_parts = expected_output.split()

        if len(actual_parts) < 2 or len(expected_parts) < 2:
            return False

        actual_weights = normalize_double(actual_parts[-1])
        expected_weights = normalize_double(expected_parts[-1])

        return actual_parts[:-1] == expected_parts[:-1] and actual_weights == expected_weights
    else:
        # 默认严格比较
        actual_normalized = " ".join(normalize_double(part) for part in actual_output.split())
        expected_normalized = " ".join(normalize_double(part) for part in expected_output.split())
        return actual_normalized.strip() == expected_normalized.strip()


def run_test(input_file, expected_output_file, executable="a.out"):
    """
    运行单个测试用例并验证输出，同时运行Valgrind检测内存泄漏。
    """
    try:
        # 读取输入命令和预期输出
        with open(input_file, 'r') as in_file, open(expected_output_file, 'r') as out_file:
            input_lines = in_file.readlines()
            expected_lines = out_file.readlines()

        # 运行程序并捕获输出
        process = subprocess.run(
            [f"./{executable}"],
            stdin=open(input_file, 'r'),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        actual_output_lines = process.stdout.strip().split("\n")

        if len(actual_output_lines) != len(expected_lines):
            return f"Test Failed: Line count mismatch.\nExpected: {len(expected_lines)} lines\nGot: {len(actual_output_lines)} lines."

        for i, (actual_line, expected_line) in enumerate(zip(actual_output_lines, expected_lines)):
            command = input_lines[i].split()[0]
            if not compare_output(command, actual_line.strip(), expected_line.strip()):
                return (
                    f"Test Failed at line {i + 1}:\n"
                    f"Command: {command}\n"
                    f"Expected: {expected_line.strip()}\n"
                    f"Actual: {actual_line.strip()}\n"
                )

        # 运行Valgrind检测内存泄漏
        valgrind_result = subprocess.run(
            ["valgrind", "--leak-check=full", f"./{executable}"],
            stdin=open(input_file, 'r'),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        valgrind_output = valgrind_result.stderr

        if "no leaks are possible" in valgrind_output:
            valgrind_result_msg = "Valgrind Passed: No memory leaks detected."
        else:
            valgrind_result_msg = (
                "Valgrind Failed: Memory leaks detected.\n"
                f"Valgrind Output:\n{valgrind_output}\n"
            )

        return f"Test Passed: Output matches expected output.\n{valgrind_result_msg}"

    except Exception as e:
        return f"Error running test: {e}"


def run_all_tests(directory, executable="a.out"):
    """
    自动检测目录中的所有测试文件并运行测试。
    """
    test_cases = find_test_files(directory)
    if not test_cases:
        print("No test cases found.")
        return

    for in_file, out_file in test_cases:
        print(f"Running test: {os.path.basename(in_file)}")
        result = run_test(in_file, out_file, executable)
        print(result)
        print("-" * 80)


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("Usage: python test_script.py <test_files_directory>")
        sys.exit(1)

    test_files_directory = sys.argv[1]
    run_all_tests(test_files_directory)
