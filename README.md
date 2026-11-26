# AIM 2627 C++ OOP Coursework 02

仓库拣货机器人任务分配系统。全部任务规格见 [Problem.pdf](Problem.pdf)，本 README 只负责本地启动。

```bash
cmake -S . -B build
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
./build/debug_task
```

## 修改范围

- 可以修改：`include/`、`src/`、`README.md`、`docs/`。
- 不要修改：`tests/`、`tools/`、`.github/`、`CMakeLists.txt`、`Problem.pdf`。
- `src/debug/main.cpp` 是 Q4 的独立调试任务；不要通过绕开缺陷的输出方式得分。

## ROS 2 演示

ROS 2 部分在 `ros2_ws/` 中独立构建，复用 `include/` 和 `src/` 的核心实现。课程使用 ROS 2 Jazzy，演示不依赖硬件。命令见题面 Q2。

## 文档交付

在 `docs/README.md` 和 `docs/DESIGN.md` 中完成 Q5。初始文件只是提示骨架，正式要求以题面为准。

## Agent 会话归档

允许并鼓励合理使用 AI 辅助开发，但必须保留可核对的过程记录。参照 Python CW 与 C++ Assessment 01，本作业使用 Agent Session Commit（AgentLedger）v0.1.5。

在个人学生仓库根目录配置，不要在教师完整目录或其他项目中安装：

```bash
python -m venv .venv-agent-session-commit
source .venv-agent-session-commit/bin/activate
python -m pip install --index-url https://pypi.org/simple 'agent-session-commit[pre-commit]==0.1.5'
agent-session-commit install --pre-commit
agent-session-commit status
```

每次正常 `git commit` 后，post-commit hook 会把新增 AI 会话打包到 `.agent-sessions/bundles/` 并 amend 到同一提交。`.agent-sessions/bundles/` 必须提交；`.agent-sessions/source/` 与 `.venv-agent-session-commit/` 已忽略。没有使用 AI 时，在 README 末尾明确声明并提供开发记录。

提交前检查：

```bash
git rev-parse HEAD
git show --stat HEAD -- .agent-sessions/
```

## AI 使用与开发过程（你来写）

声明是否使用 AI；如使用，说明工具、帮助范围、哪些建议未采用、如何验证结果，并说明 `.agent-sessions/bundles/` 中的归档位置。未使用则明确声明并提供可核对的开发记录；记录不完整时如实说明原因。
