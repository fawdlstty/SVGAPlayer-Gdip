#include "SvgaPrivate.h"



namespace SvgaLib {
	static _IImageEngineSingleton_t *__ImageEngine_t__get_obj () {
		static SVGALIB_IMAKEENGINE_IMPL s_gdip;
		return &s_gdip;
	}

	Image_t *ImageEngine_t::LoadFromMemory (const char *_data, size_t _size) {
		return __ImageEngine_t__get_obj ()->LoadFromMemory (_data, _size);
	}

	Window_t ImageEngine_t::CreatePreviewWindow (int32_t _width, int32_t _height, std::function<void ()> _close_cb) {
		return __ImageEngine_t__get_obj ()->CreatePreviewWindow (_width, _height, _close_cb);
	}

	int ImageEngine_t::Run (Window_t _wnd) {
		return __ImageEngine_t__get_obj ()->Run (_wnd);
	}

	bool ImageEngine_t::PaintImage (Window_t _wnd, Image_t *_img) {
		return __ImageEngine_t__get_obj ()->PaintImage (_wnd, _img);
	}

	Image_t *ImageEngine_t::CreateImage (int32_t _width, int32_t _height, bool _transparent) {
		return __ImageEngine_t__get_obj ()->CreateImage (_width, _height, _transparent);
	}

	void ImageEngine_t::FreeImage (Image_t *_img) {
		__ImageEngine_t__get_obj ()->FreeImage (_img);
	}
}
