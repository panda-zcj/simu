#!/usr/bin/env python
# coding=utf-8

'''
Author: pikaqiu_zcj
Date: 2024-06-29
LastEditors: pikaqiu_zcj
LastEditTime: 2024-07-21
FilePath: \script\install_dependcies.py
Description: 
Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
'''


import platform
import os
import shutil
from pythonLib import *

################# add submodule ################
# git submodule add  -f https://gitclone.com/github.com/google/googletest.git thirdlib/googletest
# git submodule add  -f https://gitclone.com/github.com/nlohmann/json.git thirdlib/json
# git submodule add  -f https://gitclone.com/github.com/Iyengar111/NanoLog.git thirdlib/NanoLog
# git submodule add  -f https://gitclone.com/github.com/abumq/easyloggingpp.git thirdlib/easyloggingpp
# git submodule add  -f https://gitclone.com/github.com/gabime/spdlog.git thirdlib/spdlog

################# delete submodule #############
# git submodule deinit -f thirdlib/googletest
# git submodule deinit -f thirdlib/json
# git submodule deinit -f thirdlib/NanoLog
# git submodule deinit -f thirdlib/easyloggingpp
# git submodule deinit -f thirdlib/spdlog
# git rm -f thirdlib/googletest
# git rm -f thirdlib/json
# git rm -f thirdlib/NanoLog
# git rm -f thirdlib/easyloggingpp
# git rm -f thirdlib/spdlog
# rm -rf .git/modules/thirdlib/googletest
# rm -rf .git/modules/thirdlib/json
# rm -rf .git/modules/thirdlib/NanoLog
# rm -rf .git/modules/thirdlib/easyloggingpp
# rm -rf .git/modules/thirdlib/spdlog


def compilerNanoLog():
    repository_path = os.path.join(ProjectDir, "thirdlib", "NanoLog")
    include_path = os.path.join(ProjectDir, "include", "NanoLog")
    src_path = os.path.join(ProjectDir, "src", "NanoLog")
    if not os.path.exists(include_path):
        os.makedirs(include_path)
    shutil.copyfile(os.path.join(repository_path, "NanoLog.hpp"),
                    os.path.join(include_path, "NanoLog.hpp"))
    if not os.path.exists(src_path):
        os.makedirs(src_path)
    shutil.copyfile(os.path.join(repository_path, "NanoLog.cpp"),
                    os.path.join(src_path, "NanoLog.cpp"))
    command.command("echo 【Attention】the origin source({}) must be modified!!!!!!".format(os.path.join(src_path, "NanoLog.cpp")))


def compilerEasyloggingpp():
    repository_path = os.path.join(ProjectDir, "thirdlib", "easyloggingpp")
    include_path = os.path.join(ProjectDir, "include", "easyloggingpp")
    src_path = os.path.join(ProjectDir, "src", "easyloggingpp")
    if not os.path.exists(os.path.join(repository_path, "build")):
        os.makedirs(os.path.join(repository_path, "build"))
    if platform.system().lower() == "linux":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && make install".format(ProjectDir), cwd=repository_path, shell=True)
    elif platform.system().lower() == "windows":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && ninja install".format(ProjectDir), cwd=repository_path, shell=True)
    cmake_path = os.path.join(ProjectDir, "share")
    if os.path.exists(cmake_path):
        for root, _, files in os.walk(cmake_path):
            for file in files:
                # 构建源文件路径和目标文件路径
                source_file = os.path.join(root, file)
                destination_file = os.path.join(
                    ProjectDir, "lib", os.path.relpath(source_file, cmake_path))

                # 复制文件（保持目录结构）
                if not os.path.exists(os.path.dirname(destination_file)):
                    os.makedirs(os.path.dirname(destination_file))
                shutil.copy2(source_file, destination_file)
        shutil.rmtree(cmake_path)

    if not os.path.exists(include_path):
        os.makedirs(include_path)
    shutil.copyfile(os.path.join(repository_path, "src", "easylogging++.h"),
                    os.path.join(include_path, "easylogging++.h"))
    if not os.path.exists(src_path):
        os.makedirs(src_path)
    shutil.copyfile(os.path.join(repository_path, "src", "easylogging++.cc"),
                    os.path.join(src_path, "easylogging++.cc"))
    if os.path.exists(os.path.join(ProjectDir, "include", "easylogging++.h")):
        os.remove(os.path.join(ProjectDir, "include", "easylogging++.h"))
    if os.path.exists(os.path.join(ProjectDir, "include", "easylogging++.cc")):
        os.remove(os.path.join(ProjectDir, "include", "easylogging++.cc"))


def compilerSpdlog():
    repository_path = os.path.join(ProjectDir, "thirdlib", "spdlog")
    if not os.path.exists(os.path.join(repository_path, "build")):
        os.makedirs(os.path.join(repository_path, "build"))
    if platform.system().lower() == "linux":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && make && make install".format(ProjectDir), cwd=repository_path, shell=True)
    elif platform.system().lower() == "windows":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && ninja && ninja install".format(ProjectDir), cwd=repository_path, shell=True)


def compilerLog():
    # compilerNanoLog()
    # compilerEasyloggingpp()
    # compilerSpdlog()
    pass

def compilerJson():
    repository_path = os.path.join(ProjectDir, "thirdlib", "json")
    if not os.path.exists(os.path.join(repository_path, "build")):
        os.makedirs(os.path.join(repository_path, "build"))
    if platform.system().lower() == "linux":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && make && make install".format(ProjectDir), cwd=repository_path, shell=True)
    elif platform.system().lower() == "windows":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && ninja && ninja install".format(ProjectDir), cwd=repository_path, shell=True)
    cmake_path = os.path.join(ProjectDir, "share")
    if os.path.exists(cmake_path):
        for root, _, files in os.walk(cmake_path):
            for file in files:
                # 构建源文件路径和目标文件路径
                source_file = os.path.join(root, file)
                destination_file = os.path.join(
                    ProjectDir, "lib", os.path.relpath(source_file, cmake_path))

                # 复制文件（保持目录结构）
                if not os.path.exists(os.path.dirname(destination_file)):
                    os.makedirs(os.path.dirname(destination_file))
                shutil.copy2(source_file, destination_file)
        shutil.rmtree(cmake_path)


def compilerGtest():
    repository_path = os.path.join(ProjectDir, "thirdlib", "googletest")
    if not os.path.exists(os.path.join(repository_path, "build")):
        os.makedirs(os.path.join(repository_path, "build"))
    if platform.system().lower() == "linux":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && make && make install".format(ProjectDir), cwd=repository_path, shell=True)
    elif platform.system().lower() == "windows":
        command.command(
            "cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && ninja && ninja install".format(ProjectDir), cwd=repository_path, shell=True)


def downloadThirdRepository():
    command.command("git submodule update --init --depth 1",
                    cwd=ProjectDir, shell=True)
    command.command("git submodule update --init --recursive --depth 1",
                    cwd=ProjectDir, shell=True)


def compilerThirdRepository():
    compilerGtest()
    compilerJson()
    compilerLog()


if __name__ == "__main__":
    if platform.system().lower() == "linux":
        log.info("[INSTALL] start install C/C++ dependcies in linux")
        downloadThirdRepository()
        compilerThirdRepository()
    elif platform.system().lower() == "windows":
        log.info("[INSTALL] start install C/C++ dependcies in windows")
        downloadThirdRepository()
        compilerThirdRepository()
