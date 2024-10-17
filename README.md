# 2024 我航 6 系《编译原理》实验仓库 ~~(预习)~~

## 前言

本义是暑假预习然后写完的，但是暑假去 happy 了。

## RoadMap

- [x] 熟悉 LLVM-IR 语法及其特性。
- [x] 熟悉 C++√/Rust× 的其中一门语言
- [x] 大致熟悉 CMake
- [x] 词法分析
- [x] 语法分析
- [ ] 语义分析
- [ ] 代码生成
- [ ] 代码优化
- [ ] MIPS 生成
- [ ] 剩余优化

### 熟悉 llvm-IR

观察定义、数组、函数、循环、条件等语句在 clang 下编译出来的结果。

### 熟悉 C++

主要参考了 [modern cpp tutorial](https://changkun.de/modern-cpp/)，一个非常简洁
的 modern c++ 教程，缺点就是太简洁了，信息密度太高，记不住。不过好处在于确实该讲的都讲到了。

## 参考

- 教程
**[buaa-se-compiling(2021)](https://buaa-se-compiling.github.io/miniSysY-tutorial/)**
**[pku-minic](https://pku-minic.github.io/online-doc/#/)**

- 参考实现
**[素未谋面的 roife学长](https://github.com/roife/racoon)**
**[课程实例编译器](https://github.com/wokron/tolangc)**

- 参考资料
**[clang_official](https://clang.llvm.org/)**
**[llvm_repository](https://github.com/llvm/llvm-project/)**

### 实验心得

#### Lab0 - 文法解读

- 几乎毫无压力

#### Lab1 - 词法分析

个人认为就是石山代码，不过想必大家都半斤八两。看了一下 llvm Project，
最关键的解读下一个 Token 的函数 `LexTokenInternal` 也写了七八百行，
所以我写的应该也算过得去吧。

#### Lab2 - 语法分析

个人写的最难受的一集。算下来写了 5 天左右（其实从重构算起的话也就一天多
一点）。这一部分建议参考其他编译器写，而且由于 SysY 文法定义的并不舒服，
在一些小细节上要多花时间。到头来写的代码没有一点 modern c++ 的风格，
时间还花了不少，属于是得不偿失。