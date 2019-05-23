#!/usr/bin/env bash

cd `dirname "$0"`
navicat_root=`pwd`

# Wine environment variables
WINEDIR="wine"
export LANG="en_US.UTF-8"
export PATH="$navicat_root/$WINEDIR/bin":"$navicat_root":"$navicat_root/$WINEDIR/drive_c/windows":"$PATH"
export LD_LIBRARY_PATH="$navicat_root/$WINEDIR/lib":"$navicat_root/lib":"$LD_LIBRARY_PATH"
export WINEDLLPATH="$navicat_root/$WINEDIR/lib/wine"
export WINELOADER="$navicat_root/$WINEDIR/bin/wine64"
export WINESERVER="$navicat_root/$WINEDIR/bin/wineserver"
export WINEPREFIX="$HOME/.navicat64"

# 获取家目录（例如"/home/tom/"）之后的路径
path_after_first_slash=${navicat_root:1}
second_slash_index=`expr index $path_after_first_slash "/"`
path_after_sencod_slash=${path_after_first_slash:$second_slash_index}
third_slash_index=`expr index $path_after_sencod_slash "/"`
path_after_third_slash=${path_after_sencod_slash:$third_slash_index}
# 将斜线替换为反斜线
path_back_slash=${path_after_third_slash/\//\\}

exec "${WINELOADER:-wine}" "navicat-patcher.exe" 'Y:\'$path_back_slash'\Navicat'
