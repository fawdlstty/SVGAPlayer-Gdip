#include "SvgaPrivate.h"



namespace SvgaLib {
	class SvgaVideoImpl_t: public ISvgaVideo_t {
	public:
		SvgaVideoImpl_t () { m_run.store (false); }
		virtual ~SvgaVideoImpl_t () { Stop (); }
		bool IsPlaying () override { return m_run.load (); }
		std::tuple<float, float> GetSize () override { return { m_width, m_height }; }

		void StartPlay (std::function<void (Image_t *)> _callback) override {
			Stop ();
			m_callback = _callback;
			m_run.store (true);
			m_thread = std::make_unique<std::thread> ([this] () {
				auto _tp = std::chrono::system_clock::now ();
				int64_t _frame = 0;
				while (m_run.load ()) {
					Image_t *_img = ImageEngine_t::CreateImage ((int32_t) m_width, (int32_t) m_height, false);
					Gdiplus::Graphics _g (_img);
					//
					for (size_t i = 0; i < m_sprites.size (); ++i) {
						std::shared_ptr<ISvgaVideoSprite_t> _sprite = m_sprites [i];
						std::shared_ptr<ISvgaVideoSpriteFrame_t> _sprite_frame = _sprite->GetFrame (_frame % _sprite->GetFrameCount ());
						Image_t *_img2 = nullptr;
						bool _dynamic = false;
						if (m_dynamic_images.find (_sprite->GetImageKey ()) != m_dynamic_images.end ()) {
							_dynamic = true;
							_img2 = m_dynamic_images [_sprite->GetImageKey ()];
						} else {
							_dynamic = false;
							_img2 = m_images [_sprite->GetImageKey ()];
						}
						ResType_t _rtype;
						std::tie (_rtype, _img2) = _sprite_frame->Clip (_img2);
						if (_img2) {
							// transform
							Transform_t _tf = _sprite_frame->GetTransform ();
							Gdiplus::Matrix m { _tf.m_a, _tf.m_b, _tf.m_c, _tf.m_d, _tf.m_tx, _tf.m_ty };
							_g.SetTransform (&m);

							// transparent
							Gdiplus::ColorMatrix _cm_transparent {
								1, 0, 0, 0, 0,
								0, 1, 0, 0, 0,
								0, 0, 1, 0, 0,
								0, 0, 0, _sprite_frame->GetAlpha (), 0,
								0, 0, 0, 0, 1,
							};
							Gdiplus::ImageAttributes _ia_transparent;
							_ia_transparent.SetColorMatrix (&_cm_transparent);

							// draw
							Gdiplus::Rect _dest_rect { 0, 0, (INT) _img2->GetWidth (), (INT) _img2->GetHeight () };
							_g.DrawImage (_img2, _dest_rect, 0, 0, _dest_rect.Width, _dest_rect.Height, Gdiplus::UnitPixel, &_ia_transparent);
							if (_rtype == ResType_t::NeedRelease)
								ImageEngine_t::FreeImage (_img2);
						}
					}
					//
					m_callback (_img);
					ImageEngine_t::FreeImage (_img);
					++_frame;
					std::this_thread::sleep_until (_tp + std::chrono::milliseconds (1000 * _frame / m_fps));
				}
			});
		}

		void Stop () override {
			if (m_run.load ()) {
				m_run.store (false);
				if (m_thread) {
					if (m_thread->joinable ())
						m_thread->join ();
					m_thread = nullptr;
				}
			}
		}

		std::string m_version = "";
		float m_width = 0.0f, m_height = 0.0f;
		int32_t m_fps = 0, m_frames = 0;
		std::map<std::string, Image_t *> m_images;
		std::map<std::string, Image_t *> m_dynamic_images;
		std::vector<std::shared_ptr<ISvgaVideoSprite_t>> m_sprites;
		//
		std::atomic<bool> m_run;
		std::unique_ptr<std::thread> m_thread;
		std::function<void (Image_t *)> m_callback;
	};



	std::shared_ptr<ISvgaVideo_t> __parse_SvgaVideo_t_v2 (const com::opensource::svga::MovieEntity &_me) {
		auto _ptr = std::make_shared<SvgaVideoImpl_t> ();
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
}
