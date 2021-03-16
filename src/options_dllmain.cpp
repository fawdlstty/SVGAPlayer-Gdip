#ifdef _WIN32

#include "SvgaPrivate.h"



int main () {
	Window_t _wnd = SvgaLib::ImageEngine_t::CreatePreviewWindow (640, 480);
	std::thread ([_wnd] () {
		std::chrono::milliseconds (100);
		//auto _video = SvgaLib::SvgaLoader_t::LoadFromFile ("E:\\GitHub\\SVGA-Samples\\Rocket.svga"); // v1
		auto _video = SvgaLib::SvgaLoader_t::LoadFromFile ("E:\\GitHub\\SVGA-Samples\\posche.svga"); // v2
		_video->StartPlay ([_wnd, _video] (Image_t *_img) {
			if (!SvgaLib::ImageEngine_t::PaintImage (_wnd, _img)) {
				_video->Stop ();
			}
		});
	}).detach ();
	return SvgaLib::ImageEngine_t::Run (_wnd);
}

int APIENTRY WinMain (HINSTANCE inst, HINSTANCE pvinst, LPSTR cmdline, int cmdshow) {
	return main ();
}



BOOL CALLBACK DllMain (HINSTANCE inst, DWORD reason, LPVOID reserved) {
	return TRUE;
}

#endif
