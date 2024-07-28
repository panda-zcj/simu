# 项目开发、测试、交付、管理的敏捷流程


## 1. 快速入门

- [ ] 目录结构说明
- [ ] VSCode任务说明
- [x] 必要的软件安装说明，例如MSYS2
- [x] cmake的doxygen配置
- [ ] asan地址消毒（GCC自带工具解决）
- [ ] 覆盖率（GCC自带工具解决）
- [x] 代码规范检查

## 2. 开发目标

- [ ] 基于事件触发的仿真器

## 3. 开发环境

### 3.1. 开发一致性部署

提供基于**终端**运行的部署脚本，快速部署开发环境**依赖的软件**。

- `Windows`端的**终端**推荐使用[MSYS2](https://www.msys2.org/)，提供与`Linux`环境一致的开发体验。
    * MSYS2（`Minimal System 2`）是用于Microsoft Windows的软件发布与开发平台，基于MinGW-w64与Cygwin，把类Unix环境中的软件移植到Windows。
    * MSYS2 提供了`bash shell`、版本控制软件（git/hg）、`MinGW-w64`工具链、以及Arch Linux的软件包管理系统`Pacman`（也是与`Cygwin`的区别）。
    * `Git Bash`即是MSYS2的精简版。
    * [VSCode和MSYS2搭建攻略](https://zhuanlan.zhihu.com/p/401188789#SnippetTab)

### 3.2. 版本构建工具

考虑跨平台、上手难度、资料丰富度，选择`cmake`，除此之外，还存在以下构建工具，可以根据需求选择

1. **Make**: Make是最古老和最常见的构建工具之一，用于自动化软件构建过程。Makefile文件描述了项目的依赖关系和构建规则。

2. **Autotools**: Autotools是一组由autoconf、automake和libtool组成的工具集，用于跨平台软件的配置和构建。

3. **Gradle**: Gradle是一个基于Groovy的构建工具，广泛用于Java项目，但也可以用于其他语言。

4. **Apache Ant**: Ant是一个基于Java的构建工具，使用XML文件来描述构建过程。它通常与Java项目一起使用，但也可以用于其他语言。

5. **Apache Maven**: Maven是另一个基于Java的构建工具，它使用XML文件描述项目结构、依赖关系和构建过程。

6. **SCons**: SCons是一个用Python编写的构建工具，它使用Python脚本来描述构建过程和依赖关系。

7. **Meson**: Meson是一个用Python编写的构建工具，专注于速度和易用性。它支持多种编程语言，并且具有简洁的语法。

8. **Bazel**: Bazel是由Google开发的构建工具，旨在支持大型项目和高度并行的构建过程。

9. **GNU Build System (GNU Build System)**: GNU Build System包括Autoconf、Automake和Libtool等工具，用于配置和构建GNU软件。

10. **Ninja**: Ninja是一个快速的构建系统，设计用于大型项目的高效构建。

### 3.3. python开发环境

为了保证python包的一致性，采用venv虚拟运行环境，与系统python环境隔离

### 3.4. C/C++开发环境

* C语言采用`C11`标准
* C++语言采用`C++11`标准
* [在线编译器网站，支持不同汇编对比](https://godbolt.org/)
* 在线对比两段代码的速度：[Quick C++ Benchmark](https://quick-bench.com/q/eP40RY6zDK-eJFdSSPBINa0apTM)。
    * 基于Google的benchmark框架，需要学习google benchmark的语法才能使用

### 3.5. 代码注释文档构建工具

采用广泛使用的doxygen工具，便于集成


## 4. 测试环境一致性

1. 测试环境采用docker构建，保障一致性

2. 代码质量管理平台采用SonarQube
    * C/C++
        * 覆盖率检查采用GCC
        * 地址消毒采用GCC，嵌入式MCU开发不需要地址消毒功能
        * 静态代码检查工具采用cppcheck
    * python
        * 采用pyflakes

3. 代码规范
    * C/C++
        * 采用clang-tidy，可以自定义代码检查规范
    * python
        * [企业级Python代码静态扫描-代码规范、逻辑、语法、安全检查，以及代码自动编排介绍](https://mp.weixin.qq.com/s?__biz=MzIyMzQ5MTY4OQ==&mid=2247486725&idx=1&sn=8b3bf7eccb2714d8f9d8f701df676fb4&chksm=e81c2e03df6ba715c190d1d998964f38aef89f61cd2ea0ee5a7dbd105bd7251f9696105d5a7f&token=215691005&lang=zh_CN&scene=21#wechat_redirect)

4. 持续集成

    - [ ] 能够本地启动CI环境的用例测试
    - [ ] 能够代码合并启动CI环境的用例测试，并拦截失败的测试用例
    - [ ] 日常用例回归测试
    - [ ] 能够将测试用例生成的代码质量数据集成到`数据度量面板`

## 5. 项目管理

1. 项目跟踪管理工具

    - [ ] [开源项目管理工具](https://www.cnblogs.com/worktile/p/17888281.html)

2. 项目文档

    - 部署服务器推荐使用docker实现
        * 基于k8s编排工具、快速部署服务
    - Wiki部分采用mkdoc实现
    - 附上项目开发计划，开发进度、开发质量的链接

3. 版本管理
    - 基于`Gitlab`版本管理实现


