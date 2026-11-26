#!/usr/bin/env python3
import re
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def run(command):
    return subprocess.run(command, cwd=ROOT, text=True, capture_output=True)


def todo_count():
    count = 0
    for directory in (ROOT / "include", ROOT / "src"):
        for path in directory.rglob("*"):
            if path.is_file() and path.suffix in {".hpp", ".cpp"}:
                count += len(re.findall(r"TODO: implement", path.read_text()))
    return count


def ctest(name):
    return run(["ctest", "--test-dir", "build", "-R", f"^{name}$", "--output-on-failure"])


def grep(pattern, paths):
    return run(["grep", "-R", "-E", pattern, *paths])


def file_lines(path):
    file_path = ROOT / path
    if not file_path.is_file():
        return 0
    return len(file_path.read_text().splitlines())


def status_icon(passed):
    return "✅" if passed else "⚠️"


def main():
    build = run(["cmake", "--build", "build", "--parallel", "2"])
    debug = ctest("debug_task")
    structure = run(["python3", "tools/validate_structure.py"])
    remaining = todo_count()
    public = ctest("public_tests") if remaining == 0 else None

    ros_files = [
        "ros2_ws/src/warehouse_dispatch/CMakeLists.txt",
        "ros2_ws/src/warehouse_dispatch/package.xml",
        "ros2_ws/src/warehouse_dispatch/launch/warehouse_demo.launch.py",
        "ros2_ws/src/warehouse_dispatch/src/simulator_node.cpp",
        "ros2_ws/src/warehouse_dispatch/src/dispatcher_node.cpp",
    ]
    ros_structure = all((ROOT / path).is_file() for path in ros_files)
    ros_core = grep("aim_oop_core", ["ros2_ws/src/warehouse_dispatch/CMakeLists.txt"]).returncode == 0
    ros_runtime = run(["./build/debug_task"])
    debug_output_ok = ros_runtime.returncode == 0 and ros_runtime.stdout.strip() == "S1 no 5 yes 0"

    readme_lines = file_lines("docs/README.md")
    design_lines = file_lines("docs/DESIGN.md")
    agent_declaration = "AI 使用与开发过程" in (ROOT / "README.md").read_text()

    sections = []
    sections.append(("Q1", 25, build.returncode == 0 and public is not None and public.returncode == 0,
                     f"TODO 剩余 {remaining}；公开测试 {'已运行' if public is not None else '未运行'}。"))
    sections.append(("Q2", 20, ros_structure and ros_core,
                     "ROS 工作区结构完整且链接核心库；运行时六事件序列由私有 grader 验收。"))
    sections.append(("Q3", 20, structure.returncode == 0,
                     "四命名空间与必需文件结构检查通过。"))
    sections.append(("Q4", 20, debug.returncode == 0 and debug_output_ok,
                     f"debug_task 输出：{ros_runtime.stdout.strip() or '<empty>'}"))
    sections.append(("Q5", 15, readme_lines >= 20 and design_lines >= 20 and agent_declaration,
                     f"README {readme_lines} 行 / DESIGN {design_lines} 行 / AI 声明{'存在' if agent_declaration else '缺失'}。"))

    lines = [
        "## C++ OOP Coursework 2 · 分题状态",
        "",
        "| 模块 | 分值 | 公开 CI 状态 | 证据与边界 |",
        "|---|---:|---|---|",
    ]
    for name, score, passed, evidence in sections:
        lines.append(f"| {name} | {score} | {status_icon(passed)} {'通过' if passed else '待完成'} | {evidence} |")

    lines.extend([
        "",
        "### 公开 CI 实际执行了什么",
        "",
        "1. 变更白名单检查：只允许 `include/**`、`src/**`、`docs/**`、`README.md`、`.agent-sessions/bundles/**`。",
        "2. CMake 构建：验证 C++17 编译、链接与目标生成。",
        "3. 结构检查：验证 Q1/Q3 必需文件、include guard、头文件 using namespace、领域反向依赖和 ROS 核心源码复用。",
        "4. Debug 检查：运行 `debug_task`，并比对 Q4 目标输出。",
        "5. 公开测试：TODO 全部完成后运行 `public_tests`；初始 TODO 阶段跳过，隐藏测试仍由私有 grader 执行。",
        "6. ROS 检查：公开 CI 只做包结构与核心库链接检查；六事件运行时序列不在公开 runner 安装 ROS 2。",
        "",
        "### 下一步",
        "",
    ])
    if remaining:
        lines.append(f"- Q1/Q3 还有 {remaining} 个实现 TODO，优先完成核心库后公开测试会自动运行。")
    else:
        lines.append("- Q1/Q3 公开检查已运行，请继续关注私有 grader 的隐藏测试结果。")
    if not debug_output_ok:
        lines.append("- Q4 修复 `submit` 的事务顺序，使输出为 `S1 no 5 yes 0`。")
    if readme_lines < 20 or design_lines < 20:
        lines.append("- Q5 文档尚未达到公开完整性检查的信息量，补齐运行、设计、验证与排查记录。")
    if not agent_declaration:
        lines.append("- README 缺少“AI 使用与开发过程”声明。")
    lines.extend([
        "",
        "### 提交",
        "",
        "- [AIM 2627 CPP Coursework 2 回执](https://docs.qq.com/form/page/DWmRmVXVVeVVkUExs)",
        "- 最终隐藏评分在私有 grader 仓库执行。",
    ])

    (ROOT / "summary.md").write_text("\n".join(lines), encoding="utf-8")
    print("\n".join(lines))


if __name__ == "__main__":
    main()
