#ifndef __SVGALIB_H__
#define __SVGALIB_H__

#include <cstdint>
#include <functional>
#include <memory>
#include <string>



namespace SvgaLib {
	class SvgaObject: public std::enable_shared_from_this<SvgaObject> {
		SvgaObject () = default;

	public:
		~SvgaObject () { Stop (); }
		void StartPlay (std::function<void ()>);
		void Stop ();
		bool IsPlaying ();

		static std::shared_ptr<SvgaObject> LoadFromMemory (const uint8_t* _data, size_t _size);
		static std::shared_ptr<SvgaObject> LoadFromFile (const std::string _path);
	};
}

#endif //__SVGALIB_H__
