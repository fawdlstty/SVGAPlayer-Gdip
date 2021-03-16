#include "SvgaPrivate.h"



namespace SvgaLib {
	std::shared_ptr<SvgaVideoSpriteFrame_t> __parse_SvgaVideoSpriteFrame_t_v2 (const com::opensource::svga::FrameEntity &_fe) {
		auto _ptr = std::make_shared<SvgaVideoSpriteFrame_t> ();
		_ptr->m_alpha = _fe.alpha ();
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
		if (_fe.has_layout ()) 	{
			x = _fe.layout ().x ();
			y = _fe.layout ().y ();
			w = _fe.layout ().width ();
			h = _fe.layout ().height ();
		}
		_ptr->m_layout = RectF_t (x, y, w, h);
		//
		float a = 1.0f;
		float b = 0.0f;
		float c = 0.0f;
		float d = 1.0f;
		float tx = 0.0f;
		float ty = 0.0f;
		if (_fe.has_transform ()) {
			a = _fe.transform ().a ();
			b = _fe.transform ().b ();
			c = _fe.transform ().c ();
			d = _fe.transform ().d ();
			tx = _fe.transform ().tx ();
			ty = _fe.transform ().ty ();
		}
		_ptr->m_transform = Transform_t (a, b, c, d, tx, ty);
		//
		_ptr->m_clipPath = _fe.clippath ();
		return _ptr;
	}



	SvgaVideoSpriteFrame_t::SvgaVideoSpriteFrame_t () {
	}



	SvgaVideoSpriteFrame_t::~SvgaVideoSpriteFrame_t () {
	}
}
