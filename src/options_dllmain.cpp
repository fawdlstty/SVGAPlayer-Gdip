#ifdef _WIN32

#include "SvgaLib.h"



BOOL CALLBACK DllMain (HINSTANCE inst, DWORD reason, LPVOID reserved) {
//	if (reason == DLL_PROCESS_ATTACH) {
//#ifdef SVGALIB_USE_GDIPLUS
//		OptionsGdip_t::init ();
//#endif
//	} else if (reason == DLL_THREAD_ATTACH) {
//	} else if (reason == DLL_THREAD_DETACH) {
//	} else if (reason == DLL_PROCESS_DETACH) {
//#ifdef SVGALIB_USE_GDIPLUS
//		OptionsGdip_t::release ();
//#endif
//	}
	return TRUE;
}

#endif
