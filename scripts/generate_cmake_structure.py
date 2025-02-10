#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import datetime

def create_file_tree(startpath, output_file):
    output_file.write("# CMake 项目文件结构\n")
    output_file.write(f"生成时间: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
    
    for root, dirs, files in os.walk(startpath):
        # 跳过build、bin和vendor目录
        if 'build' in dirs:
            dirs.remove('build')
        if 'bin' in dirs:
            dirs.remove('bin')
        if 'vendor' in dirs:
            dirs.remove('vendor')
            
        level = root.replace(startpath, '').count(os.sep)
        indent = '  ' * level
        output_file.write(f"{indent}{os.path.basename(root)}/\n")
        subindent = '  ' * (level + 1)
        for f in files:
            output_file.write(f"{subindent}{f}\n")

def read_file_content(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            return f.read()
    except UnicodeDecodeError:
        try:
            with open(filepath, 'r', encoding='gbk') as f:
                return f.read()
        except:
            return "<<无法读取文件>>"

def write_cmake_files(startpath, output_file):
    output_file.write("\n\n# CMake 文件内容\n\n")
    
    for root, dirs, files in os.walk(startpath):
        # 跳过build、bin和vendor目录
        if 'build' in dirs:
            dirs.remove('build')
        if 'bin' in dirs:
            dirs.remove('bin')
            
        for f in files:
            if f.endswith(('CMakeLists.txt', '.cmake')):
                filepath = os.path.join(root, f)
                rel_path = os.path.relpath(filepath, startpath)
                
                output_file.write(f"## {rel_path}\n")
                output_file.write("```cmake\n")
                output_file.write(read_file_content(filepath))
                output_file.write("\n```\n\n")

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    engine_dir = os.path.join(os.path.dirname(script_dir), 'Engine')
    output_path = os.path.join(script_dir, 'cmake_documentation.md')
    
    with open(output_path, 'w', encoding='utf-8') as f:
        create_file_tree(engine_dir, f)
        write_cmake_files(engine_dir, f)

if __name__ == "__main__":
    main() 