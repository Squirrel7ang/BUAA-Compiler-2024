```bash
# 生成可执行文件
$ clang main.c -o main
# 查看编译的过程
$ clang -ccc-print-phases main.c

# 生成 tokens
$ clang -E -Xclang -dump-tokens main.c
# 生成语法树
$ clang -fsyntax-only -Xclang -ast-dump main.c
# 生成 llvm ir（不开优化）
$ clang -S -emit-llvm main.c -o main.ll -O0

# 生成汇编（在本实验中用处不大）
$ clang -S main.c -o main.s
# 生成目标文件（在本实验中用处不大）
$ clang -c main.c -o main.o
```