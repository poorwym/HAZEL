@echo off
chcp 65001 > nul

:: 确保在正确的目录中执行
cd /d %~dp0\..

:: 使用项目中的premake5
set PREMAKE=.\vendor\bin\premake5.exe

:: 检查premake5是否存在
if not exist "%PREMAKE%" (
    echo 错误: 未找到premake5,请确保vendor\bin\premake5.exe文件存在
    echo 当前目录: %CD%
    dir .\vendor\bin
    pause
    exit /b 1
)

:: 清理功能
:clean
echo 正在清理上一次生成的文件...
if exist bin rmdir /s /q bin
if exist bin-int rmdir /s /q bin-int
if exist *.sln del /f /q *.sln
if exist *.vcxproj del /f /q *.vcxproj
if exist *.vcxproj.filters del /f /q *.vcxproj.filters
if exist *.vcxproj.user del /f /q *.vcxproj.user

:: 检查是否需要清理
if "%1"=="clean" goto :generate
if not "%1"=="" goto :help

:generate
:: 创建必要的目录结构
echo 创建目录结构...
if not exist bin\Debug-windows-x86_64\Sandbox mkdir bin\Debug-windows-x86_64\Sandbox
if not exist bin\Release-windows-x86_64\Sandbox mkdir bin\Release-windows-x86_64\Sandbox
if not exist bin-int mkdir bin-int

:: 生成Visual Studio解决方案
echo 正在生成Visual Studio解决方案...
%PREMAKE% vs2022

echo Visual Studio解决方案生成完成!
goto :end

:help
echo 用法:
echo build.bat        - 生成项目文件
echo build.bat clean  - 清理并重新生成项目文件
goto :end

:end
pause 