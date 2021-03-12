cmake_minimum_required (VERSION 3.15)
project ("SvgaLib" LANGUAGES CXX)

add_library (SvgaLib SHARED "svga.pb.cc" "options_dllmain.cpp" "SvgaLoader.cpp" "options_gdip.hpp" "options_interface.h" "SvgaPrivate.h")

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(SvgaLib PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

find_package(ZLIB REQUIRED)
target_link_libraries(SvgaLib PRIVATE ZLIB::ZLIB)
