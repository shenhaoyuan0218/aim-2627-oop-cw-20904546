from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ERRORS = []


def main() -> int:
    required = [
        "include/warehouse/domain.hpp",
        "include/dispatch/policy.hpp",
        "include/dispatch/scheduler.hpp",
        "include/audit/trace.hpp",
        "include/app/cli.hpp",
        "src/warehouse/domain.cpp",
        "src/dispatch/policy.cpp",
        "src/dispatch/scheduler.cpp",
        "src/audit/trace.cpp",
        "src/app/cli.cpp",
        "ros2_ws/src/warehouse_dispatch/CMakeLists.txt",
    ]
    missing = [path for path in required if not (ROOT / path).is_file()]
    if missing:
        ERRORS.append("missing required files: " + ", ".join(missing))

    for path in (ROOT / "include").rglob("*.hpp"):
        lines = [line for line in path.read_text().splitlines() if line.strip()]
        relative = path.relative_to(ROOT)
        if any("using namespace" in line for line in lines):
            ERRORS.append(f"using namespace in {relative}")
        if len(lines) < 2 or "#ifndef" not in lines[0] or "#define" not in lines[1]:
            ERRORS.append(f"missing or malformed include guard in {relative}")

    domain = ROOT / "include/warehouse/domain.hpp"
    if domain.is_file():
        text = domain.read_text()
        if "dispatch/" in text or "rclcpp" in text:
            ERRORS.append("warehouse/domain.hpp has a forbidden dispatch/ROS dependency")

    ros_cmake = ROOT / "ros2_ws/src/warehouse_dispatch/CMakeLists.txt"
    if ros_cmake.is_file():
        text = ros_cmake.read_text()
        if "warehouse_core" not in text or "src/warehouse/domain.cpp" not in text:
            ERRORS.append("ROS package does not compile the shared core sources")

    if ERRORS:
        print("Structure check failed:")
        print("\n".join(f"- {error}" for error in ERRORS))
        return 1
    print("Structure OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
