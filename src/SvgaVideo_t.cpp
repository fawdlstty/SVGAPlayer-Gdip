#include "SvgaPrivate.h"



namespace SvgaLib {
	std::shared_ptr<SvgaVideo_t> __parse_SvgaVideo_t_v2 (const com::opensource::svga::MovieEntity &_me) {
		auto _ptr = std::make_shared<SvgaVideo_t> ();
		_ptr->m_version = _me.version ();
		_ptr->m_width = _me.params ().viewboxwidth ();
		_ptr->m_height = _me.params ().viewboxheight ();
		_ptr->m_fps = _me.params ().fps ();
		_ptr->m_frames = _me.params ().frames ();
		for (auto _iter = _me.images ().cbegin (); _iter != _me.images ().cend (); ++_iter) {
			Image_t *_bmp = ImageEngine_t::LoadFromMemory (&_iter->second [0], _iter->second.size ());
			_ptr->m_images [_iter->first] = _bmp;
		}
		_ptr->m_sprites.reserve (_me.sprites_size ());
		for (int i = 0; i < _me.sprites_size (); ++i) {
			auto _sprite = __parse_SvgaVideoSprite_t_v2 (_me.sprites (i));
			if (_sprite)
				_ptr->m_sprites.push_back (_sprite);
		}
		return _ptr;
	}



	SvgaVideo_t::SvgaVideo_t () {
	}



	SvgaVideo_t::~SvgaVideo_t () {
		Stop ();
	}



	void SvgaVideo_t::StartPlay (std::function<void (Image_t *_img)>) {
		// TODO
	}



	void SvgaVideo_t::Stop () {
		// TODO
	}



	bool SvgaVideo_t::IsPlaying () {
		// TODO
		return false;
	}
}
