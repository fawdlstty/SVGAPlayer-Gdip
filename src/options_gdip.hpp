#ifndef __OPTIONS_GDIP_HPP__
#define __OPTIONS_GDIP_HPP__

#define NOMINMAX
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <Windows.h>
#ifndef min
#	define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#	define max(a,b) ((a)>(b)?(a):(b))
#endif
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")
#undef min
#undef max

#include "options_interface.h"



namespace SvgaLib {
	class ImageEngineGdip_t : public _IImageEngineSingleton_t {
	public:
		ImageEngineGdip_t () {
			Gdiplus::GdiplusStartupInput _input;
			Gdiplus::GdiplusStartupOutput _output;
			Gdiplus::GdiplusStartup (&s_token, &_input, &_output);
		}

		virtual ~ImageEngineGdip_t () {
			Gdiplus::GdiplusShutdown (s_token);
		}

		Gdiplus::Bitmap *LoadFromMemory (const char *_data, size_t _size) override {
			IStream *_stm = nullptr;
			CreateStreamOnHGlobal (NULL, TRUE, &_stm);
			_stm->Seek (LARGE_INTEGER { 0 }, STREAM_SEEK_SET, NULL);
			_stm->SetSize (ULARGE_INTEGER { 0 });
			ULONG _ulsize = 0;
			_stm->Write (_data, (ULONG) _size, &_ulsize);
			Gdiplus::Bitmap *_bmp = Gdiplus::Bitmap::FromStream (_stm);
			_stm->Release ();
			return _bmp;
		}

	private:
		ULONG_PTR s_token;
	};
}

#endif //__OPTIONS_GDIP_HPP__
