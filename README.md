项目结构（模块化说明）

- `sorting/`：排序算法实现（每个算法一个源文件），公共头文件 `sort.h` 和工具 `utils.c`。
- `tests/`：测试/演示/基准驱动程序（如 `show_sorts.c`, `sort_benchmark_simple.c`, `main.c`）。
- `data/`：输入/输出样例数据文件（如 `n0.in`, `n1.in`, `n5.in`，以及生成的 `out_n*.txt`）。

快速开始 - 必做项

1) 演示（展示三种排序：冒泡、选择、插入）

  - 构建并运行演示（自动生成 n=0,1,5 输入并写入 data/）：

    ```bash
    make run_show_sorts
    ```

  - 查看结果：

    ```bash
    cat data/out_n0.txt
    cat data/out_n1.txt
    cat data/out_n5.txt
    ```

  - 如果要手动运行并输入：

    ```bash
    make show_sorts
    ./show_sorts    # 然后键盘输入或重定向 data/n5.in
    ```

2) 程序鲁棒性（n=0,1,5）

  - 已包含于 `make run_show_sorts` 的自动运行中（会生成 `data/out_n*.txt`）。

3) 性能分析（至少 3 个算法）

  - 基准（ARRAY_SIZE=5000）已实现：

    ```bash
    make bench5000
    less bench5000.out
    ```

  - 如果想比较 Quick/Merge/Heap 请让我修改 `tests/sort_benchmark_simple.c` 以同时测试三者并输出 CSV。

进阶（选做，打桩与 perf）

A) 基于共享库的打桩（建议流程）

  - 生成共享库：

    ```bash
    make lib   # 生成 libsort.so
    ```

  - 编写或使用已有 `clock_stub.c`（仓库中已有 `clock_stub.c`，需要编译为共享库），示例：

    ```bash
    gcc -shared -fPIC -o clock_stub.so clock_stub.c -ldl -lrt
    LD_PRELOAD=./clock_stub.so ./your_program
    ```

  - 说明：通过 `LD_PRELOAD` 或运行时链接替换排序函数符号，`clock_stub` 在函数入口/出口处调用 `clock_gettime` 记录单次调用时间（满足 n>=65536 的测量要求）。

B) perf 方法（需要在系统上安装 perf）

  - 安装（Debian/Ubuntu）：

    ```bash
    sudo apt update
    sudo apt install linux-tools-common linux-tools-$(uname -r)
    ```

  - 运行 perf（示例）：

    ```bash
    perf stat -e cycles,instructions,cache-misses ./bench65536
    perf record -o perf.data ./bench65536
    perf report -i perf.data
    ```

C) VSCode 插件

  - 推荐安装：BenchIt、CodeTime（视插件可用性而定）。可以在 `.vscode/tasks.json` 中添加 `make bench5000` 的任务以便一键运行。

代码优化（可选）

- 我可以为某个排序（例如 Heap/Quick）实现优化版本，修改 `tests/sort_benchmark_simple.c` 来在同一场景下比较优化前后的性能，并输出 CSV/图表数据。

如果你同意，我可以立刻执行下列一项：
- A：把基准扩展为比较 Quick/Merge/Heap 并生成 CSV 报表
- B：实现 `make bench65536` 并实现基于共享库的 `clock_stub`，展示单次调用计时
- C：为某个算法实现 CPU/Cache 优化并加入基准对比
- D：生成 VSCode `tasks.json` 示例和更详细 README

选择一项让我继续即可。

演示给老师的逐步清单（推荐）

1) 准备

  - 打开终端并切换到项目根目录：

    ```bash
    cd /home/zixing/Desktop/Lab3
    git status   # 可选，确保没有未提交改动
    ```

2) 演示模块化项目结构（30s）

  - 在编辑器中打开项目树，展示 `sorting/`、`tests/`、`data/` 三个目录结构。
  - 简要说明：每个排序算法一个文件，通过 `sorting/sort.h` 提供统一接口，测试/基准在 `tests/`。

3) 功能演示（学生演示流程，约 1-2 分钟）

  - 运行演示并展示 n=0,1,5 输出：

    ```bash
    make run_show_sorts
    cat data/out_n0.txt
    cat data/out_n1.txt
    cat data/out_n5.txt
    ```

  - 若教师随机指定一个算法（例如“选择排序”），学生可以直接用 `show_sorts.c` 的交互模式演示：

    ```bash
    make show_sorts
    ./show_sorts   # 学生键盘输入 n 和数据，或重定向 data/n5.in
    ```

4) 性能演示（约 1-3 分钟）

  - 运行基准（5000）并展示 CSV：

    ```bash
    make bench5000
    less bench5000.out
    cat bench5000.csv
    ```

  - 如需展示单次调用在大规模下的测量（打桩方法）：

    ```bash
    make bench65536
    make clock_stub.so
    LD_PRELOAD=./clock_stub.so ./bench65536 > bench65536.out 2>&1
    cat sort_timing.txt
    ```

5) 结束与答疑

  - 总结性能结论（例如：在随机数据下 Quick 最快；逆序时 Merge 更稳定；优化后的 Quick 在多数场景下明显优于基本实现），并回答老师提问。

VSCode 一键任务

 - 已添加 `.vscode/tasks.json`，可以在 VSCode 中一键运行：`Run demo (show_sorts)`、`Run bench5000`、`Run bench65536 (LD_PRELOAD)`。

