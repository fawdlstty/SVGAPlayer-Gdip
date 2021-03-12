#ifndef __OPTIONS_INTERFACE_H__
#define __OPTIONS_INTERFACE_H__

#include "SvgaLib.h"



namespace SvgaLib {
	class _IImageEngineSingleton_t {
	public:
		_IImageEngineSingleton_t () = default;
		virtual ~_IImageEngineSingleton_t () = default;

		virtual Image_t LoadFromMemory (const char *_data, size_t _size) = 0;
	};
}

#endif //__OPTIONS_INTERFACE_H__
