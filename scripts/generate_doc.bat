@echo off
chcp 65001 > nul

python scripts/generate_engine_doc.py

echo 文档生成完成！
pause 