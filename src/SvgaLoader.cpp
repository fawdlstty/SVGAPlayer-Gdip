#include "SvgaLib.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

#include "svga.pb.h"

//#include "../zipper/zipper/unzipper.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>



std::shared_ptr<SvgaLib::SvgaAnimation> SvgaLib::SvgaLoader::LoadFromMemory (const char *_bytes, size_t _size) {
	if (_size < 4)
		return nullptr;
	if (_bytes [0] == (char) 80 && _bytes [1] == (char) 75 && _bytes [2] == (char) 3 && _bytes [3] == (char) 4) {
		// version 1
		//zipper::Unzipper _unz (_bytes);
	} else {
		// version 2
		std::stringstream _ss;
		_ss.write (_bytes, (std::streamsize) _size);
		google::protobuf::io::IstreamInputStream _iis (&_ss);
		google::protobuf::io::GzipInputStream _gis (&_iis);
		com::opensource::svga::MovieEntity _entity;
		if (!_entity.ParseFromZeroCopyStream (&_gis))
			return nullptr;
		auto _ptr = std::make_shared<SvgaLib::SvgaAnimation> ();
		_ptr->m_version = _entity.version ();
		_ptr->m_width = _entity.params ().viewboxwidth ();
		_ptr->m_height = _entity.params ().viewboxheight ();
		_ptr->m_fps = _entity.params ().fps ();
		_ptr->m_frames = _entity.params ().frames ();
		for (auto _iter = _entity.images ().cbegin (); _iter != _entity.images ().cend (); ++_iter) {
			Gdiplus::Bitmap *_bmp = ImageEngine_t::LoadFromMemory (&_iter->second [0], _iter->second.size ());
			_ptr->m_images[_iter->first] = _bmp;
		}
	}
	return nullptr;
}



std::shared_ptr<SvgaLib::SvgaAnimation> SvgaLib::SvgaLoader::LoadFromFile (const std::string _path) {
	std::ifstream _ifs (_path, std::ios::binary);
	//std::string str ((std::istreambuf_iterator<char> (_ifs)), std::istreambuf_iterator<char> ());
	_ifs.seekg (0, std::ios::end);
	size_t _fsz = (size_t) _ifs.tellg ();
	_ifs.seekg (0, std::ios::beg);
	std::vector<char> _bytes;
	_bytes.resize (_fsz);
	_ifs.read (&_bytes [0], (std::streamsize) _fsz);
	_ifs.close ();
	return LoadFromMemory (&_bytes [0], _bytes.size ());
}



#ifdef _WIN32
std::shared_ptr<SvgaLib::SvgaAnimation> SvgaLib::SvgaLoader::LoadFromResourceA (HINSTANCE _inst, LPCSTR _type, LPCSTR _name) {
	std::shared_ptr<SvgaLib::SvgaAnimation> _ret = nullptr;
	HRSRC hRsrc = ::FindResourceA (_inst, _name, _type);
	if (hRsrc) {
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource (_inst, hRsrc);
		if (hGlobal) {
			DWORD dwSize = ::SizeofResource (_inst, hRsrc);
			if (dwSize) {
				LPBYTE _bin_data = (LPBYTE)::LockResource (hGlobal);
				if (_bin_data)
					_ret = LoadFromMemory ((const char *) _bin_data, (size_t) dwSize);
			}
		}
		::FreeResource (hRsrc);
	}
	return _ret;
}



std::shared_ptr<SvgaLib::SvgaAnimation> SvgaLib::SvgaLoader::LoadFromResourceW (HINSTANCE _inst, LPCWSTR _type, LPCWSTR _name) {
	std::shared_ptr<SvgaLib::SvgaAnimation> _ret = nullptr;
	HRSRC hRsrc = ::FindResourceW (_inst, _name, _type);
	if (hRsrc) {
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource (_inst, hRsrc);
		if (hGlobal) {
			DWORD dwSize = ::SizeofResource (_inst, hRsrc);
			if (dwSize) {
				LPBYTE _bin_data = (LPBYTE)::LockResource (hGlobal);
				if (_bin_data)
					_ret = LoadFromMemory ((const char *) _bin_data, (size_t) dwSize);
			}
		}
		::FreeResource (hRsrc);
	}
	return _ret;
}
#endif
