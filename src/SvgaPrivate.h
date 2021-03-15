#ifndef __SVGA_PRIVATE_H__
#define __SVGA_PRIVATE_H__

#include "options_ImageEngineGdip_t.hpp"
#include "SvgaLib.h"
#include "svga.pb.h"



//#include "../zipper/zipper/unzipper.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>



#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

namespace SvgaLib {
	std::shared_ptr<SvgaVideoSpriteFrame_t> __parse_SvgaVideoSpriteFrame_t_v2 (const com::opensource::svga::FrameEntity &_fe);
	std::shared_ptr<SvgaVideoSprite_t> __parse_SvgaVideoSprite_t_v2 (const com::opensource::svga::SpriteEntity &_se);
	std::shared_ptr<SvgaVideo_t> __parse_SvgaVideo_t_v2 (const com::opensource::svga::MovieEntity &_me);
}

#endif //__SVGA_PRIVATE_H__
