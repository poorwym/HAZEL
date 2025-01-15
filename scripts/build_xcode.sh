#!/bin/bash

# 确保脚本在出错时停止执行
set -e

# 使用项目中的premake5
PREMAKE="./vendor/bin/premake5"

# 检查premake5是否存在
if [ ! -f "$PREMAKE" ]; then
    echo "错误: 未找到premake5,请确保vendor/bin/premake5文件存在"
    exit 1
fi

# 清理上一次生成的文件
echo "正在清理上一次生成的文件..."
rm -rf bin/
rm -rf bin-int/
rm -rf *.xcodeproj
rm -rf *.xcworkspace
rm -rf Makefile
rm -rf *.make

# 创建必要的目录结构
echo "创建目录结构..."
mkdir -p bin/Debug-macosx-x86_64/Sandbox
mkdir -p bin/Release-macosx-x86_64/Sandbox
mkdir -p bin-int

# 生成Xcode项目
echo "正在生成Xcode项目..."
$PREMAKE xcode4

echo "Xcode项目生成完成!"
echo "现在你可以打开Hazel.xcworkspace进行开发了" 