#!/usr/bin/env python
# coding=utf-8

'''
Author: pikaqiu_zcj
Date: 2024-04-09
LastEditors: pikaqiu_zcj
LastEditTime: 2024-07-31
FilePath: \script\test.py
Description: 
Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
'''

import platform
from pythonLib import *

if __name__ == "__main__":
    if platform.system().lower() == "linux":
        log.info("[BUILD] start build objects in linux")
        command.command("cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && ninja simu_test&& cd {}/bin && simu_test".format(ProjectDir, ProjectDir),
                        cwd=ProjectDir, shell=True, ignore=True)
    elif platform.system().lower() == "windows":
        log.info("[BUILD] start build objects in windows")
        command.command("cd build && rm * -rf && cmake .. -DCMAKE_INSTALL_PREFIX={} && ninja simu_test && cd {}/bin && simu_test.exe".format(ProjectDir, ProjectDir),
                        cwd=ProjectDir, shell=True, ignore=True)
