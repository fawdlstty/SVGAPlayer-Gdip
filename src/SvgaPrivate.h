#ifndef __SVGA_PRIVATE_H__
#define __SVGA_PRIVATE_H__

#include "options_ImageEngineGdip_t.hpp"
#include "SvgaLib.h"
#include "svga.pb.h"



#include "../zipper/zipper/unzipper.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>



#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <tuple>
#include <vector>

namespace SvgaLib {
	//std::shared_ptr<ISvgaVideoSpriteFrame_t>	__parse_SvgaVideoSpriteFrame_t_v2 (const com::opensource::svga::FrameEntity &_fe);
	//std::shared_ptr<ISvgaVideoSprite_t>			__parse_SvgaVideoSprite_t_v2 (const com::opensource::svga::SpriteEntity &_se);
	//std::shared_ptr<ISvgaVideo_t>				__parse_SvgaVideo_t_v1 (std::vector<uint8_t> &_movie_spec);

	std::shared_ptr<ISvgaVideoSpriteFrame_t>	__parse_SvgaVideoSpriteFrame_t_v2 (const com::opensource::svga::FrameEntity &_fe);
	std::shared_ptr<ISvgaVideoSprite_t>			__parse_SvgaVideoSprite_t_v2 (const com::opensource::svga::SpriteEntity &_se);
	std::shared_ptr<ISvgaVideo_t>				__parse_SvgaVideo_t_v2 (const com::opensource::svga::MovieEntity &_me);
}

#endif //__SVGA_PRIVATE_H__
