@echo off
chcp 65001 > nul

python scripts/generate_engine_doc.py
python scripts/generate_cmake_structure.py

echo 文档生成完成！
pause 