# SvgaLib

基于GDI+，用于绘制SVGA动画的C++库。（备注：仅SVGA2.0）

## 描述

前置条件：

1. 通过vcpkg安装protobuf、zlib

测试方式：

1. 修改options_dllmain.cpp中的svga文件路径
2. 使用VS2019（或ninja）编译运行

作为库来使用：

1. 修改 `CMakeLists.txt`，将 `add_library` 这一行取消注释，将 `add_executable` 这一行注释掉
2. 使用VS2019（或ninja）编译

## 目标

[ ] Svga 1.0
[x] Svga 2.0
[ ] Skia
[x] Gdi+
[ ] Flutter Windows 插件

<!--
#生成 protobuf 文件命令：
protoc -I ./SVGA-Format/proto --cpp_out=./src svga.proto
-->
