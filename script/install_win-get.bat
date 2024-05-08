@echo off

chcp 65001 > nul

echo 正在检查是否已安装 Windows 包管理器（winget）...

:: 使用 PowerShell 命令检查 winget 是否已安装
powershell -Command if (Get-Command winget -ErrorAction SilentlyContinue) { exit 0 } else { exit 1 }

:: 检查 PowerShell 命令的退出代码
if %errorlevel% equ 0 (
    echo Windows 包管理器（winget）已安装
) else (
    echo Windows 包管理器（winget）未安装

    echo 正在下载安装程序...
    :: 使用 PowerShell 命令下载安装 winget
    powershell -Command Invoke-WebRequest -Uri https://github.com/microsoft/winget-cli/releases/download/v1.7.11132/Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.msixbundle -OutFile Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.appxbundle

    echo 正在安装 Windows 包管理器（winget）...
    :: 使用 PowerShell 命令安装 winget
    powershell -Command Add-AppxPackage .\Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.appxbundle

    :: 删除下载的安装程序
    del .\Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.appxbundle
    
    echo Windows 包管理器（winget）安装完成
)

