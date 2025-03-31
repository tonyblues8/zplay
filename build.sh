#!/bin/bash
#set -x
shellpath=`pwd`
[[ -f ./zdelv ]] && echo "Deleting zdelv..." && rm ./zdelv

make clean && make && [[ -f ./zdelv ]] || { echo "编译失败"; exit 1; }
chmod a+x ./zdelv

echo "zdelv编译成功"
if [ -d "/Applications/熊猫下载.app/Contents" ]; then
    # 拷贝 zdelv 到指定目录
    cp -f ./zdelv "/Applications/熊猫下载.app/Contents/MacOS/"
    echo "copy zdelv to /Applications/熊猫下载.app/Contents/MacOS/ oKKKKK"

    # 拷贝 zdvq 到指定目录
    cp -f "/Applications/熊猫剪切.app/Contents/Resources/zdvq" "/Applications/熊猫下载.app/Contents/Resources/"
    echo "copy zdvq to /Applications/熊猫下载.app/Contents/Resources/ oKKKKK"
fi

if [ -d "/Applications/熊猫剪切.app/Contents" ]; then
    mv -f ./zdelv /Applications/熊猫剪切.app/Contents/MacOS/
    echo "mv zdelv to /Applications/熊猫剪切.app/Contents/MacOS/ oKKKKK"
fi

APP="/Applications/熊猫剪切.app"
#FRAMEWORKS_PATH="$APP/Contents/Frameworks"
FRAMEWORKS_PATH="$APP/Contents/MacOS"
EXECUTABLE_PATH="$APP/Contents/MacOS/zdelv"

# 使用 otool 获取非系统依赖列表
echo "分析可执行文件的动态库依赖..."
#DEPENDENCIES=$(otool -L "$EXECUTABLE_PATH" | tail -n +2 | awk '/\// {print $1}' | grep -Ev '^/usr/lib|^/System/Library')
DEPENDENCIES=$(otool -L "$EXECUTABLE_PATH" | sed -n '2,$p' | awk '/\// {print $1}' | grep -Ev '^/usr/lib|^/System/Library')

if [ -z "$DEPENDENCIES" ]; then
    echo "没有非系统库依赖。"
    #exit 0
else
    echo "非系统库依赖列表:"
    echo "$DEPENDENCIES"
    for DEP in $DEPENDENCIES; do
        LIB_BASENAME=$(basename "$DEP")
        TARGET_PATH="$FRAMEWORKS_PATH/$LIB_BASENAME"
        if [ ! -f "$TARGET_PATH" ]; then
            #echo "cp $TARGET_PATH"
            echo "复制: $DEP -> $TARGET_PATH"
            cp "$DEP" "$FRAMEWORKS_PATH/"
            echo "更新动态库路径:$DEP -> @executable_path"
            install_name_tool -change "$DEP" @executable_path/$LIB_BASENAME "$EXECUTABLE_PATH"
        fi
    done
    echo "所有动态库处理完成"
fi
