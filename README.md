# SvgaLib

����GDI+�����ڻ���SVGA������C++�⡣����ע����SVGA2.0��

## ����

ǰ��������

1. ͨ��vcpkg��װprotobuf��zlib

���Է�ʽ��

1. �޸�options_dllmain.cpp�е�svga�ļ�·��
2. ʹ��VS2019����ninja����������

��Ϊ����ʹ�ã�

1. �޸� `CMakeLists.txt`���� `add_library` ��һ��ȡ��ע�ͣ��� `add_executable` ��һ��ע�͵�
2. ʹ��VS2019����ninja������

## Ŀ��

[ ] Svga 1.0
[x] Svga 2.0
[ ] Skia
[x] Gdi+
[ ] Flutter Windows ���

<!--
#���� protobuf �ļ����
protoc -I ./SVGA-Format/proto --cpp_out=./src svga.proto
-->
