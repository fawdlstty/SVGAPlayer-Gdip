cmake_minimum_required (VERSION 3.15)
project ("SvgaLib" LANGUAGES CXX)

add_library (SvgaLib SHARED "svga.pb.cc" "options_dllmain.cpp" "SvgaLoader_t.cpp" "ImageEngine_t.cpp" "SvgaVideo_t.cpp" "SvgaVideoSprite_t.cpp" "SvgaVideoSpriteFrame_t.cpp")

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(SvgaLib PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

find_package(ZLIB REQUIRED)
target_link_libraries(SvgaLib PRIVATE ZLIB::ZLIB)
