#ifndef __OPTIONS_IMAGEENGINEGDIP_HPP__
#define __OPTIONS_IMAGEENGINEGDIP_HPP__

#define NOMINMAX
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <Windows.h>
#include <tchar.h>
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
	Transform_t& operator= (const Transform_t &_obj) {
		m_a = _obj.m_a;
		m_b = _obj.m_b;
		m_c = _obj.m_c;
		m_d = _obj.m_d;
		m_tx = _obj.m_tx;
		m_ty = _obj.m_ty;
		return *this;
	}
	float m_a, m_b, m_c, m_d, m_tx, m_ty;
};
typedef HWND Window_t;

#include "options_IImageEngineSingleton_t.h"



namespace SvgaLib {
	class ImageEngineGdip_t : public _IImageEngineSingleton_t {
	public:
		ImageEngineGdip_t () {
			Gdiplus::GdiplusStartupInput _input;
			Gdiplus::GdiplusStartupOutput _output;
			Gdiplus::GdiplusStartup (&m_token, &_input, &_output);
		}

		virtual ~ImageEngineGdip_t () {
			if (m_wcex.lpfnWndProc) {
				::RegisterClassEx (&m_wcex);
				m_wcex.lpfnWndProc = NULL;
			}
			//
			Gdiplus::GdiplusShutdown (m_token);
		}

		Gdiplus::Bitmap* LoadFromMemory (const char *_data, size_t _size) override {
			IStream *_stm = nullptr;
			HRESULT _result = CreateStreamOnHGlobal (NULL, TRUE, &_stm);
			if (_result == E_INVALIDARG || _result == E_OUTOFMEMORY)
				return nullptr;
			_stm->Seek (LARGE_INTEGER { 0 }, STREAM_SEEK_SET, NULL);
			_stm->SetSize (ULARGE_INTEGER { 0 });
			ULONG _ulsize = 0;
			_stm->Write (_data, (ULONG) _size, &_ulsize);
			Gdiplus::Bitmap *_bmp = Gdiplus::Bitmap::FromStream (_stm);
			_stm->Release ();
			return _bmp;
		}

		Window_t CreatePreviewWindow (int32_t _width, int32_t _height) override {
			if (!m_wcex.lpfnWndProc) {
				m_wcex.style = CS_HREDRAW | CS_VREDRAW;
				m_wcex.lpfnWndProc = WndProc;
				m_wcex.cbClsExtra = 0;
				m_wcex.cbWndExtra = 0;
				m_wcex.hInstance = (HINSTANCE) ::GetModuleHandle (NULL);
				m_wcex.hIcon = NULL;
				m_wcex.hCursor = LoadCursor (nullptr, IDC_ARROW);
				m_wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
				m_wcex.lpszMenuName = NULL;
				m_wcex.lpszClassName = _T ("SvgaLib_Preview");
				m_wcex.hIconSm = NULL;
				ATOM _reg = ::RegisterClassEx (&m_wcex);
				if (!_reg) {
					m_wcex.lpfnWndProc = NULL;
					return NULL;
				}
			}
			HWND hWnd = ::CreateWindowExA (0L, _T ("SvgaLib_Preview"), _T ("SvgaLib Preview"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, m_wcex.hInstance, NULL);
			if (!hWnd)
				return NULL;
			::ShowWindow (hWnd, SW_SHOW);
			::UpdateWindow (hWnd);
			return hWnd;
		}

		int Run (Window_t _wnd) override {
			MSG _msg { 0 };
			m_running = true;
			while (::GetMessage (&_msg, _wnd, 0, 0)) {
				if (!_msg.hwnd)
					break;
				::TranslateMessage (&_msg);
				::DispatchMessage (&_msg);
			}
			m_running = false;
			return _msg.wParam;
		}

		bool PaintImage (Window_t _wnd, Image_t *_img) override {
			if (!m_running)
				return false;
			HDC _hdc = ::GetDC (_wnd);
			Gdiplus::Graphics _g (_hdc);
			Gdiplus::Rect _rc { 0, 0, _img->GetWidth (), _img->GetHeight () };
			_g.DrawImage (_img, _rc, 0, 0, _img->GetWidth (), _img->GetHeight (), Gdiplus::UnitPixel);
			return true;
		}

	private:
		ULONG_PTR m_token;
		WNDCLASSEX m_wcex { sizeof (WNDCLASSEX) };
		bool m_running = false;

		static LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			//if (uMsg == WM_CLOSE) {
			//	::CloseWindow (hWnd);
			//	return ::DefWindowProc (hWnd, uMsg, wParam, lParam);
			//} else if (uMsg == WM_DESTROY) {
			//	::PostQuitMessage (0);
			//} else {
			//	return ::DefWindowProc (hWnd, uMsg, wParam, lParam);
			//}
			//return 0;
			return ::DefWindowProc (hWnd, uMsg, wParam, lParam);
		}
	};
}

#endif //__OPTIONS_IMAGEENGINEGDIP_HPP__
