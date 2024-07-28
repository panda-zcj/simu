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
set /p install_path="Please enter software install path [%defaultValue%]:":
:: 检查用户是否输入了值，如果未输入则使用默认值
if "%install_path%"=="" set "install_path=%defaultValue%"
echo [1;31m if installer.exe is support, You software install path: %install_path% [0m

pause

@REM :: 安装代码注释提取工具链 Doxygen & Graphviz
@REM winget install --id DimitriVanHeesch.Doxygen --rainbow --interactive --version 1.10.0 --location %install_path%
@REM winget install --id Graphviz.Graphviz --rainbow --interactive --version 11.0.0

echo.
echo ======================== install MSYS2 ========================
echo [1;31m 存在断网才能正常安装的情况，故这里只下载不安装，需要手动安装MSYS2 [0m
echo [1;31m 请将MSYS2的安装路径适配到${workspaceFolder}\\.vscode\\settings.json配置文件的terminal.integrated.profiles.windows的path参数 [0m
echo [1;31m 请将MSYS2的安装路径适配到${workspaceFolder}\\.vscode\\settings.json配置文件的python.defaultInterpreterPath参数 [0m
echo [1;31m 请将MSYS2的安装路径适配到${workspaceFolder}\\.vscode\\c_cpp_properties.json配置文件的compilerPath参数 [0m
echo [1;31m 请将MSYS2的安装路径适配到${workspaceFolder}\\.vscode\\launch.json配置文件的miDebuggerPath参数 [0m

winget download --id MSYS2.MSYS2  --version 20240113 --download-directory %install_path%