#ifndef __SVGALIB_H__
#define __SVGALIB_H__

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#ifdef _WIN32
#	define NOMINMAX
#	define _WIN32_WINNT _WIN32_WINNT_WIN7
#	include <Windows.h>
#endif



namespace SvgaLib {
	//
	// Platform abstraction
	//

#ifndef SVGALIB_IMAKEENGINE_IMPL
	typedef void *Image_t;
	typedef void *RectF_t;
	typedef void *Transform_t;
	typedef void *Window_t;
#endif

	class ImageEngine_t {
	public:
		static Image_t* LoadFromMemory (const char *_data, size_t _size);
		static Window_t CreatePreviewWindow (int32_t _width, int32_t _height);
		static int Run (Window_t _wnd);
		static bool PaintImage (Window_t _wnd, Image_t *_img);

		static Image_t* CreateImage (int32_t _width, int32_t _height);
		static void FreeImage (Image_t *_img);
	};



	//
	// Implement
	//

	class ISvgaVideoSpriteFrame_t: public std::enable_shared_from_this<ISvgaVideoSpriteFrame_t> {
	public:
		ISvgaVideoSpriteFrame_t () = default;
		virtual ~ISvgaVideoSpriteFrame_t () = default;
	};

	class ISvgaVideoSprite_t: public std::enable_shared_from_this<ISvgaVideoSprite_t> {
	public:
		ISvgaVideoSprite_t () = default;
		virtual ~ISvgaVideoSprite_t () = default;
		virtual size_t GetFrameCount () = 0;
		virtual std::shared_ptr<ISvgaVideoSpriteFrame_t> GetFrame (size_t _index) = 0;
		virtual std::string GetImageKey () = 0;
	};

	class ISvgaVideo_t: public std::enable_shared_from_this<ISvgaVideo_t> {
	public:
		ISvgaVideo_t () = default;
		virtual ~ISvgaVideo_t () = default;
		virtual bool IsPlaying () = 0;
		virtual std::tuple<float, float> GetSize () = 0;
		virtual void StartPlay (std::function<void (Image_t*)> _callback) = 0;
		virtual void Stop () = 0;
	};

	class SvgaLoader_t {
	public:
		static std::shared_ptr<ISvgaVideo_t> LoadFromMemory (const char *_bytes, size_t _size);
		static std::shared_ptr<ISvgaVideo_t> LoadFromFile (const std::string _path);
#ifdef _WIN32
		static std::shared_ptr<ISvgaVideo_t> LoadFromResourceA (HINSTANCE _inst, LPCSTR _type, LPCSTR _name);
		static std::shared_ptr<ISvgaVideo_t> LoadFromResourceW (HINSTANCE _inst, LPCWSTR _type, LPCWSTR _name);
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
