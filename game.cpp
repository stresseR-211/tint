#define GAME
#include "preprocess.hpp"
#include "nodes.hpp"
#include "audioStream.hpp"
#include <chrono>
#include <thread>



int main(int argc, char** argv) {
	int code{0}; //return code
	watchdog([&code] {
		sys::quit();
		sys::logger::assert(std::format("shutdown {:#010x}", code));
		sys::logger::close();
	});
	sys::init();
	sys::audio::init();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	sys::audio::device dev = sys::audio::device::open("../data/temp/menu.ogg", CH::MUS, FX::NONE);
	sys::audio::changeVolume(CH::MUS, 12);
	dev.play();





	node::window window = node::create::window(sys::config::getWidth(), sys::config::getHeight());
	SDL_Event event;
	for (;;) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: return code;
			case SDL_MOUSEBUTTONDOWN:
				window.hide();
				std::this_thread::sleep_for(std::chrono::seconds(3));
				window.show();
			default: break;
			}
		}
	}
	return code = 69420; /* vamos */
}