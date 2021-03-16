#include "SvgaPrivate.h"



namespace SvgaLib {
	std::shared_ptr<SvgaVideoSprite_t> __parse_SvgaVideoSprite_t_v2 (const com::opensource::svga::SpriteEntity &_se) {
		auto _ptr = std::make_shared<SvgaVideoSprite_t> ();
		_ptr->m_image_key = _se.imagekey ();
		_ptr->m_frames.reserve (_se.frames_size ());
		for (int i = 0; i < _se.frames_size (); ++i) {
			auto _frame = __parse_SvgaVideoSpriteFrame_t_v2 (_se.frames (i));
			if (_frame)
				_ptr->m_frames.push_back (_frame);
		}
		return _ptr;
	}



	SvgaVideoSprite_t::SvgaVideoSprite_t () {
	}



	SvgaVideoSprite_t::~SvgaVideoSprite_t () {
	}
}
