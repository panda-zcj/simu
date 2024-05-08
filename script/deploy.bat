@echo off

@REM :: 替换安装源
@REM winget source remove winget 
@REM winget source add winget https://mirrors.ustc.edu.cn/winget-source
@REM :: 恢复安装源
@REM winget source reset winget

:: 设置控制台编码为 UTF-8
chcp 65001 >nul

:: 让用户输入安装路径
set "defaultValue=.\tool\toolchain\"
set /p install_path="Please enter install path [%defaultValue%]:":
:: 检查用户是否输入了值，如果未输入则使用默认值
if "%install_path%"=="" set "install_path=%defaultValue%"
echo if installer.exe is support, You install_path: [%install_path%]

pause

winget install --id Kitware.CMake --rainbow --interactive --version 3.29.2

@REM winget install --id DimitriVanHeesch.Doxygen --rainbow --interactive --version 1.10.0 --location "%install_path%"
winget install --id DimitriVanHeesch.Doxygen --rainbow --interactive --version 1.10.0

winget install --id Graphviz.Graphviz --rainbow --interactive --version 11.0.0

winget install --id MSYS2.MSYS2 --rainbow --interactive --version 20240113