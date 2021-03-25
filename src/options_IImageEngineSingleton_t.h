#ifndef __OPTIONS_INTERFACE_H__
#define __OPTIONS_INTERFACE_H__

#include <cstdint>
#include <functional>



namespace SvgaLib {
	class _IImageEngineSingleton_t {
	public:
		_IImageEngineSingleton_t () = default;
		virtual ~_IImageEngineSingleton_t () = default;
		virtual Image_t *LoadFromMemory (const char *_data, size_t _size) = 0;
		virtual Window_t CreatePreviewWindow (int32_t _width, int32_t _height, std::function<void ()> _close_cb) = 0;
		virtual int Run (Window_t _wnd) = 0;
		virtual bool PaintImage (Window_t _wnd, Image_t *_img) = 0;
		virtual Image_t *CreateImage (int32_t _width, int32_t _height, bool _transparent) = 0;
		virtual void FreeImage (Image_t *_img) = 0;
	};
}

#endif //__OPTIONS_INTERFACE_H__
