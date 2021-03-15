#ifndef __OPTIONS_IMAGEENGINEGDIP_HPP__
#define __OPTIONS_IMAGEENGINEGDIP_HPP__

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



#define SVGALIB_IMAKEENGINE_IMPL ImageEngineGdip_t
typedef Gdiplus::Bitmap Image_t;
typedef Gdiplus::RectF RectF_t;
struct Transform_t {
	Transform_t () {
		m_a = m_b = m_c = m_d = m_tx = m_ty = 0;
	}
	Transform_t (float a, float b, float c, float d, float tx, float ty)
		: m_a (a), m_b (b), m_c (c), m_d (d), m_tx (tx), m_ty (ty) {}
	Transform_t (const Transform_t &_obj) {
		m_a = _obj.m_a;
		m_b = _obj.m_b;
		m_c = _obj.m_c;
		m_d = _obj.m_d;
		m_tx = _obj.m_tx;
		m_ty = _obj.m_ty;
	}
	Transform_t &operator= (const Transform_t &_obj) {
		m_a = _obj.m_a;
		m_b = _obj.m_b;
		m_c = _obj.m_c;
		m_d = _obj.m_d;
		m_tx = _obj.m_tx;
		m_ty = _obj.m_ty;
	}
	float m_a, m_b, m_c, m_d, m_tx, m_ty;
};

#include "options_IImageEngineSingleton_t.h"



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

		Gdiplus::Bitmap* LoadFromMemory (const char *_data, size_t _size) override {
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

#endif //__OPTIONS_IMAGEENGINEGDIP_HPP__
