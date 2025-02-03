@echo off

cl

if %ERRORLEVEL% neq 0 (
  call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
)

cls

test_runner.exe

@REM rmdir /S /Q .\build
@REM mkdir build
@REM pushd .\build

@REM cl /nologo /diagnostics:caret /WX /W4 /wd4189 /wd4996 /wd4100 /wd4244 ..\string8\main.c
@REM main.exe

@REM popd