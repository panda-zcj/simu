@echo off

:: 设置控制台编码为 UTF-8
chcp 65001 >nul

@REM ::【msys2的软件包】(https://packages.msys2.org/queue)
pacman -S --needed base-devel mingw-w64-x86_64-toolchain

@REM :: 安装doxygen的vscode扩展, 添加配置
@REM :: cmake的doxygen未完成,需要额外的配置
@REM :: 配置doxyfile（https://cmake.org/cmake/help/latest/module/FindDoxygen.html）
@REM :: asan地址消毒（GCC自带工具解决）
@REM :: 覆盖率（GCC自带工具解决）