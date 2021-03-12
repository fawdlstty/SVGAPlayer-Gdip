#ifndef __SVGALIB_H__
#define __SVGALIB_H__

#define SVGALIB_USE_GDIPLUS

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

#ifdef _WIN32
#	define NOMINMAX
#	define _WIN32_WINNT _WIN32_WINNT_WIN7
#	include <Windows.h>
#endif



namespace SvgaLib {
	//
	// Platform abstraction
	//

#if defined SVGALIB_USE_GDIPLUS
	typedef Gdiplus::Bitmap *Image_t;
#else
	typedef void *Image_t;
#endif

	class ImageEngine_t {
	public:
		// TODO: 貌似可以不要这玩意
		//static void Init () { _get_obj (); }
		static Image_t LoadFromMemory (const char *_data, size_t _size) { return _get_obj ()->LoadFromMemory (_data, _size); }

	private:
		static _IImageEngineSingleton_t *_get_obj () {
#if defined SVGALIB_USE_GDIPLUS
			static ImageEngineGdip_t s_gdip;
			return &s_gdip;
#else
			return nullptr;
#endif
		}
	};



	//
	// Implement
	//

	class SvgaAnimation: public std::enable_shared_from_this<SvgaAnimation> {
	public:
		SvgaAnimation () = default;
		~SvgaAnimation () { Stop (); }
		void StartPlay (std::function<void ()>);
		void Stop ();
		bool IsPlaying ();

		std::string m_version;
		float m_width, m_height;
		int32_t m_fps, m_frames;
		std::map<std::string, Image_t> m_images;
	};

	class SvgaLoader {
	public:
		static std::shared_ptr<SvgaAnimation> LoadFromMemory (const char *_bytes, size_t _size);
		static std::shared_ptr<SvgaAnimation> LoadFromFile (const std::string _path);
#ifdef _WIN32
		static std::shared_ptr<SvgaAnimation> LoadFromResourceA (HINSTANCE _inst, LPCSTR _type, LPCSTR _name);
		static std::shared_ptr<SvgaAnimation> LoadFromResourceW (HINSTANCE _inst, LPCWSTR _type, LPCWSTR _name);
#endif
	};
}

#ifdef _WIN32
#	ifdef UNICODE
#		define LoadFromResource LoadFromResourceW
#	else
#		define LoadFromResource LoadFromResourceA
#	endif
#endif

#endif //__SVGALIB_H__
