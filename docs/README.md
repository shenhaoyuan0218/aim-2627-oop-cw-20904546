# Operation and verification

本文档面向接手维护者：从仓库根目录出发，逐条复制命令即可完成本地与 ROS 2 构建、运行和验证。正式规格见 [Problem.pdf](../Problem.pdf)。

## Build

# 仓库拣货机器人调度系统｜运行说明

本文说明如何编译 C++ 程序、运行公开测试，以及在 Ubuntu 上启动 ROS 2 演示。
每一节都从仓库根目录开始。可以先运行 `ls`，确认当前目录里有 `CMakeLists.txt`、`src`、`tools` 和 `ros2_ws`。

## 1. 本地构建与测试

需要支持 C++17 的编译器、CMake 3.16 或更高版本，以及 Python 3。

```bash
cmake -S . -B build
cmake --build build
./build/public_tests
./build/core_demo
python3 tools/validate_structure.py
```

| 检查项 | 预期输出 |
|---|---|
| `public_tests`：运行公开测试 | `PUBLIC 2/2` |
| `core_demo`：启动本地演示程序 | `AIM OOP CW2 1.0`，随后正常退出 |
| `validate_structure.py`：检查工程结构 | `Structure OK` |

也可以运行项目自带的脚本，一次完成配置、构建和 CTest 测试：

```bash
bash tools/check.sh
```

当前版本在干净构建目录中通过了两项 CTest 测试：`public_tests` 和 `debug_task`。

## 2. 运行 Q4 调试程序

完成上面的 CMake 配置后，运行：

```bash
cmake --build build --target debug_task && ./build/debug_task
```

`&&` 表示编译成功后才运行程序。如果编译失败，单独执行旧的 `debug_task` 可能仍会看到上一次构建的输出。

| 版本 | 程序输出 |
|---|---|
| 原始模板 | `S1 no -1 yes 0` |
| 修复后 | `S1 no 5 yes 0` |

重点看第三列。S2 原有 5 件货，O2 需要 6 件。拒绝 O2 后，S2 应该仍有 5 件货。完整的复现过程和边界测试见 [设计与调试记录](DESIGN.md)。

## 3. 在 Ubuntu 上启动 ROS 2 演示

课程环境为 Ubuntu 24.04、ROS 2 Jazzy 和 colcon。如果使用 Mac 通过 SSH 连接虚拟机，以下命令在 Ubuntu 终端执行。进入 Ubuntu 上的仓库根目录后，依次运行：

```bash
source /opt/ros/jazzy/setup.bash
cd ros2_ws
colcon build --symlink-install
source install/setup.bash
ros2 launch warehouse_dispatch warehouse_demo.launch.py
```

第一次 `source` 加载系统安装的 ROS 2。第二次 `source` 加载刚编译好的作业包。看到 `1 package finished` 后，再启动演示。

模拟器向 `/warehouse_events` 发送事件。调度节点收到事件后调用 C++ 核心库，再把结果发布到 `/dispatch_results`。初始库存为 S1=3、S2=2，两台机器人均空闲，使用 `priority` 策略。

| 输入事件 `/warehouse_events` | 预期输出 `/dispatch_results` |
|---|---|
| `ORDER O1 SKU-A 1` | `ACCEPT O1 S1` |
| `ORDER O2 SKU-A 1` | `ACCEPT O2 S2` |
| `ORDER O3 SKU-A 2` | `REJECT O3` |
| `COMPLETE O1` | `COMPLETE O1` |
| `ORDER O4 SKU-A 3` | `REJECT O4` |
| `WAIT 1` | `WAITED 1` |

核对日志时看每行末尾的结果。验收时应依次看到以上六行，随后两个节点自动退出，终端重新出现命令提示符。

## 4. 常见问题

- **提示 `ros2: command not found`**：在当前 Ubuntu 终端执行 `source /opt/ros/jazzy/setup.bash`，再运行 `command -v ros2`。正常情况下会显示 ROS 2 命令的路径。
- **找不到 `warehouse_dispatch` 包**：在 `ros2_ws` 目录确认 `colcon build --symlink-install` 已成功，然后在同一个终端执行 `source install/setup.bash`。用 `ros2 pkg prefix warehouse_dispatch` 检查是否能找到该包。
- **Mac 上已经改好代码，Ubuntu 仍报旧错误**：检查报错中显示的源码路径，确认修改后的文件已经同步到 Ubuntu 上对应的作业目录，再重新编译。