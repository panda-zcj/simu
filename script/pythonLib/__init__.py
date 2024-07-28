#!/usr/bin/env python
# coding=utf-8

'''
Author: pikaqiu_zcj
Date: 2024-06-22
LastEditors: pikaqiu_zcj
LastEditTime: 2024-06-22
FilePath: \script\pythonLib\__init__.py
Description: 
Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
'''


__all__ = ['command', 'log', 'ProjectDir', 'PackageName']

import os

ProjectDir = os.path.dirname(os.path.abspath(__file__)).split("script")[0]
PackageName = "PythonLib"
