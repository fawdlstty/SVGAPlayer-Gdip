#include "SvgaPrivate.h"



namespace SvgaLib {
	class SvgaVideoSpriteImpl_t: public ISvgaVideoSprite_t {
	public:
		SvgaVideoSpriteImpl_t () = default;
		virtual ~SvgaVideoSpriteImpl_t () = default;
		size_t GetFrameCount () override { return m_frames.size (); }
		std::shared_ptr<ISvgaVideoSpriteFrame_t> GetFrame (size_t _index) override { return m_frames [_index]; }
		std::string GetImageKey () override { return m_image_key; }

		std::string m_image_key = "";
		std::vector<std::shared_ptr<ISvgaVideoSpriteFrame_t>> m_frames;
	};



	std::shared_ptr<ISvgaVideoSprite_t> __parse_SvgaVideoSprite_t_v2 (const com::opensource::svga::SpriteEntity &_se) {
		auto _ptr = std::make_shared<SvgaVideoSpriteImpl_t> ();
		_ptr->m_image_key = _se.imagekey ();
		_ptr->m_frames.reserve (_se.frames_size ());
		for (int i = 0; i < _se.frames_size (); ++i) {
			auto _frame = __parse_SvgaVideoSpriteFrame_t_v2 (_se.frames (i));
			if (_frame)
				_ptr->m_frames.push_back (_frame);
		}
		return _ptr;
	}
}
