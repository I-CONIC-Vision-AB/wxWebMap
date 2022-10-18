@echo off

REM Make sure all submodules are initialized
git submodule update --init --recursive

REM Bootstrap vcpkg (ie. download the vcpkg executable)
REM vcpkg\bootstrap-vcpkg.bat

REM Install required packages
REM vcpkg\vcpkg.exe install --triplet x64-windows boost-chrono