#!/usr/bin/env python
# coding=utf-8

'''
Author: pikaqiu_zcj
Date: 2024-06-22
LastEditors: pikaqiu_zcj
LastEditTime: 2024-06-23
FilePath: \script\pythonLib\log.py
Description: 
Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
'''

__all__ = ['debug', 'info', 'warning', 'error',
           'fatal', 'setConsoleLevel', 'setFileLevel']

import os
import logging
from datetime import datetime
import colorama
if __name__ == "__main__":
    import sys
    sys.path.append(os.path.dirname(
        os.path.abspath(__file__)).split("pythonLib")[0])
import pythonLib


# 初始化 colorama
colorama.init()


class CustomStreamHandler(logging.StreamHandler):
    def emit(self, record):
        # 添加文字高亮逻辑，例如在输出前加上 ANSI 转义码
        msg = self.format(record)
        if record.levelno == logging.DEBUG:
            # Blue color for DEBUG messages
            msg = colorama.Fore.BLUE + \
                '[DEBUG]{}'.format(msg) + colorama.Style.RESET_ALL
        elif record.levelno == logging.INFO:
            # Cyan color for INFO messages
            msg = colorama.Fore.CYAN + \
                '[INFO]{}'.format(msg) + colorama.Style.RESET_ALL
        elif record.levelno == logging.WARNING:
            # Yellow color for WARNING messages
            msg = colorama.Fore.YELLOW + \
                '[WARN]{}'.format(msg) + colorama.Style.RESET_ALL
        elif record.levelno == logging.ERROR:
            # Red color for ERROR and CRITICAL messages
            msg = colorama.Fore.RED + \
                '[ERROR]{}'.format(msg) + colorama.Style.RESET_ALL
        elif record.levelno == logging.CRITICAL:
            # Red color for ERROR and CRITICAL messages
            msg = colorama.Fore.RED + \
                '[CRITICAL]{}'.format(msg) + colorama.Style.RESET_ALL

        self.stream.write(msg + self.terminator)
        self.flush()


# 创建日志记录器
logger = logging.getLogger('LOG')
logger.setLevel(logging.DEBUG)

# 创建控制台处理器并设置级别为DEBUG
console_handler = CustomStreamHandler()
console_handler.setLevel(logging.DEBUG)


# 创建文件处理器并设置级别为DEBUG
file_handler = logging.FileHandler(os.path.join(pythonLib.ProjectDir, "log", "{}.log".format(
    datetime.now().strftime('%Y-%m-%d'))), encoding='utf-8')
file_handler.setLevel(logging.DEBUG)


# 设置日志格式
console_handler.setFormatter(logging.Formatter('%(message)s'))
file_handler.setFormatter(logging.Formatter(
    '[%(asctime)s][%(levelname)s]%(message)s'))

# 将处理器添加到记录器中
logger.addHandler(file_handler)
logger.addHandler(console_handler)


def debug(msg, console=True, file=True):
    if console == False:
        logger.removeHandler(console_handler)
    if file == False:
        logger.removeHandler(file_handler)
    logger.debug(msg)
    if console == False:
        logger.addHandler(console_handler)
    if file == False:
        logger.addHandler(file_handler)


def info(msg, console=True, file=True):
    if console == False:
        logger.removeHandler(console_handler)
    if file == False:
        logger.removeHandler(file_handler)
    logger.info(msg)
    if console == False:
        logger.addHandler(console_handler)
    if file == False:
        logger.addHandler(file_handler)


def warning(msg, console=True, file=True):
    if console == False:
        logger.removeHandler(console_handler)
    if file == False:
        logger.removeHandler(file_handler)
    logger.warning(msg)
    if console == False:
        logger.addHandler(console_handler)
    if file == False:
        logger.addHandler(file_handler)


def error(msg, console=True, file=True):
    if console == False:
        logger.removeHandler(console_handler)
    if file == False:
        logger.removeHandler(file_handler)
    logger.error(msg)
    if console == False:
        logger.addHandler(console_handler)
    if file == False:
        logger.addHandler(file_handler)


def critical(msg, console=True, file=True):
    if console == False:
        logger.removeHandler(console_handler)
    if file == False:
        logger.removeHandler(file_handler)
    logger.critical(msg)
    if console == False:
        logger.addHandler(console_handler)
    if file == False:
        logger.addHandler(file_handler)


def setConsoleLevel(level):
    """
    Set the logging level of this Console. level must be an int or a str
    """
    console_handler.setLevel(level)


def setFileLevel(level):
    """
    Set the logging level of this file. level must be an int or a str
    """
    file_handler.setLevel(level)


if __name__ == "__main__":
    debug("This is a debug message", console=False)
    info("This is an info message", file=False)
    warning("This is a warning message")
    error("This is an error message")
    critical("This is a critical message")
    info("你好，世界")
