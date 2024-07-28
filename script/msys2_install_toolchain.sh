#!/bin/sh

###
 # @Author: pikaqiu_zcj
 # @Date: 2024-06-17
 # @LastEditors: pikaqiu_zcj
 # @LastEditTime: 2024-06-17
 # @FilePath: script\msys2_install_toolchain.sh
 # @Description: 
 # Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
### 

# :: pacman 换源
# sed -i "s#https\?://mirror.msys2.org/#https://mirrors.tuna.tsinghua.edu.cn/msys2/#g" /etc/pacman.d/mirrorlist*

# ::【msys2的软件包】(https://packages.msys2.org/queue)
# :: pacman -Rs package_name 同时删除软件包及其配置文件
# :: pacman -Q 列出系统中所有已安装的软件包
# :: pacman -Sc 清理系统中不再需要的缓存和残留文件
pacman -S --needed  base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-python mingw-w64-x86_64-python-pip
pacman -S --needed  mingw-w64-x86_64-graphviz mingw-w64-x86_64-graphviz-docs doxygen