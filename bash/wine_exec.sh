#!/bin/sh

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

# 两个步骤的例子
# exec "${WINELOADER:-wine}" "navicat-patcher.exe" "Y:\bin\navicat121_premium_en_x64\Navicat"
# exec "${WINELOADER:-wine}" "navicat-keygen.exe" "-text" "./RegPrivateKey.pem"

exec "${WINELOADER:-wine}" "$1" "$2" "$3"