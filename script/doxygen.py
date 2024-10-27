#!/usr/bin/env python
# coding=utf-8

'''
Author: pikaqiu_zcj
Date: 2024-07-31
LastEditors: pikaqiu_zcj
LastEditTime: 2024-08-01
FilePath: \script\doxygen.py
Description: 
Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
'''

import platform
from pythonLib import *

if __name__ == "__main__":
    if platform.system().lower() == "linux":
        log.info("[BUILD] start build objects in linux")
        command.command("cd build && rm * -rf && cmake .. -DDOXYGEN_OUTPUT_DIRECTORY={}/doc -DCMAKE_INSTALL_PREFIX={} && make simu_doc".format(ProjectDir, ProjectDir),
                        cwd=ProjectDir, shell=True)
    elif platform.system().lower() == "windows":
        log.info("[BUILD] start build objects in windows")
        ProjectDir_s = ProjectDir.split(":")
        ProjectDir_unix_style = "/{}{}doc".format(
            ProjectDir_s[0].lower(), ProjectDir_s[-1])
        ProjectDir_unix_style.replace("\\", "/")
        command.command("cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} -DDOXYGEN_OUTPUT_DIRECTORY={}".format(
            ProjectDir, ProjectDir_unix_style), cwd=ProjectDir, shell=True)
        command.command("doxygen.exe build/Doxyfile.simu_doc".format(ProjectDir), cwd=ProjectDir, shell=True)