#include "SvgaPrivate.h"



namespace SvgaLib {
	static _IImageEngineSingleton_t *__ImageEngine_t__get_obj () {
		static SVGALIB_IMAKEENGINE_IMPL s_gdip;
		return &s_gdip;
	}

	Image_t* ImageEngine_t::LoadFromMemory (const char *_data, size_t _size) {
		return __ImageEngine_t__get_obj ()->LoadFromMemory (_data, _size);
	}
}
