#ifdef _WIN32

#include "SvgaPrivate.h"



int main () {
	std::mutex _mtx;
	//auto _video = SvgaLib::SvgaLoader_t::LoadFromFile ("E:\\GitHub\\SVGA-Samples\\Rocket.svga"); // v1
	//auto _video = SvgaLib::SvgaLoader_t::LoadFromFile ("E:\\GitHub\\SVGA-Samples\\posche.svga"); // v2  angel.svga  halloween.svga
	auto _video = SvgaLib::SvgaLoader_t::LoadFromFile ("E:\\GitHub\\SVGA-Samples\\halloween.svga");
	Window_t _wnd = SvgaLib::ImageEngine_t::CreatePreviewWindow (640, 480, [&] () {
		std::unique_lock<std::mutex> ul (_mtx);
		_video->Stop ();
	});
	std::thread ([&] () {
		std::chrono::milliseconds (100);
		_video->StartPlay ([&] (Image_t *_img) {
			std::unique_lock<std::mutex> ul (_mtx);
			SvgaLib::ImageEngine_t::PaintImage (_wnd, _img);
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
