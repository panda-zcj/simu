#!/usr/bin/env python
# coding=utf-8

'''
Author: pikaqiu_zcj
Date: 2024-06-20
LastEditors: pikaqiu_zcj
LastEditTime: 2024-06-23
FilePath: \script\pythonLib\command.py
Description: 
Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
'''

__all__ = ['command']

import subprocess
import functools
import os
import signal
import traceback
if __name__ == "__main__":
    import sys
    sys.path.append(os.path.dirname(
        os.path.abspath(__file__)).split("pythonLib")[0])
import pythonLib.log


def echo(type="COMMAND"):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            pythonLib.log.info("[{}] {}".format(type, *args))
            func(*args, **kwargs)
        return wrapper
    return decorator


@echo(type="COMMAND")
def command(cmd_str, timeout=None, cwd=None, env=None, shell=False, ignore=False):
    return_code = 0
    try:
        proc = subprocess.Popen(cmd_str, cwd=cwd, env=env,
                                shell=shell, start_new_session=True)
        proc.communicate(timeout=timeout)
        return_code = proc.returncode
    except subprocess.TimeoutExpired:
        return_code = None
        pythonLib.log.error(" Timeout expired.")
    except Exception as e:
        return_code = None
        pythonLib.log.critical(" {}".format(repr(e)))
        traceback_info = traceback.format_exc()
        pythonLib.log.info(" {}".format(traceback_info), console=False)
    finally:
        try:
            # kill subprocess
            if proc.poll() is None:
                os.killpg(proc.pid, signal.SIGTERM)
        except Exception:
            pass

        if return_code != 0 and ignore == False:
            assert False


if __name__ == "__main__":
    command("echo test correct command")
    command("xxxx", ignore=True)
    command("xxxx")
