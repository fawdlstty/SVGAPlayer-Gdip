#ifndef __SVGALIB_H__
#define __SVGALIB_H__

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
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
#endif

	class ImageEngine_t {
	public:
		static Image_t* LoadFromMemory (const char *_data, size_t _size);
	};



	//
	// Implement
	//

	class SvgaVideoSpriteFrame_t: public std::enable_shared_from_this<SvgaVideoSpriteFrame_t> {
	public:
		SvgaVideoSpriteFrame_t ();
		~SvgaVideoSpriteFrame_t ();
		float m_alpha;
		RectF_t m_layout;
		Transform_t m_transform;
		std::string m_clipPath;
	};

	class SvgaVideoSprite_t: public std::enable_shared_from_this<SvgaVideoSprite_t> {
	public:
		SvgaVideoSprite_t ();
		~SvgaVideoSprite_t ();
		std::string m_image_key;
		std::vector< std::shared_ptr<SvgaVideoSpriteFrame_t>> m_frames;
	};

	class SvgaVideo_t: public std::enable_shared_from_this<SvgaVideo_t> {
	public:
		SvgaVideo_t ();
		~SvgaVideo_t ();
		void StartPlay (std::function<void ()>);
		void Stop ();
		bool IsPlaying ();

		std::string m_version;
		float m_width, m_height;
		int32_t m_fps, m_frames;
		std::map<std::string, Image_t*> m_images;
		std::vector<std::shared_ptr<SvgaVideoSprite_t>> m_sprites;
	};

	class SvgaLoader_t {
	public:
		static std::shared_ptr<SvgaVideo_t> LoadFromMemory (const char *_bytes, size_t _size);
		static std::shared_ptr<SvgaVideo_t> LoadFromFile (const std::string _path);
#ifdef _WIN32
		static std::shared_ptr<SvgaVideo_t> LoadFromResourceA (HINSTANCE _inst, LPCSTR _type, LPCSTR _name);
		static std::shared_ptr<SvgaVideo_t> LoadFromResourceW (HINSTANCE _inst, LPCWSTR _type, LPCWSTR _name);
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
