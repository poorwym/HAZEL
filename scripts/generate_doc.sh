#!/bin/bash

# 确保脚本在出错时停止执行
set -e

# 运行Python脚本
python3 generate_engine_doc.py

echo "文档生成完成！" 