@echo off

cls
where /q cl || call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

rmdir /S /Q .\build
mkdir build

cl main.cpp /nologo /W4 /WX /Z7 /fsanitize=address /wd4189 /wd4505 /wd4101 /wd4996 /wd4100 /Fo.\build\ %filename% /link /stack:0x400000,0x400000 /out:build\main.exe

build\main