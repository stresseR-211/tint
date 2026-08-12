#include "internal.hpp"
#include "core.hpp"
#include <format>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
namespace sys {
	void init(void) {
		fsCheck(); intCheck();
		if (int code = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS); code < 0) {
			logger::assert(std::format("Couldn't initialise SDL: {} [{}]", SDL_GetError(), code));
			crash(SDL_GetError());
		}
		if (int code = TTF_Init(); code < 0) {
			logger::assert(std::format("Couldn't initialise SDL_TTF: {} [{}]", TTF_GetError(), code));
			crash(TTF_GetError());
		}
		logger::assert("Initialised.");
	}
	void quit(void) {
		try {
			for (auto entry : fs::recursive_directory_iterator("../data/temp")) {
				fs::remove(entry);
			}
			TTF_Quit();
			SDL_Quit();
		} catch (const std::exception& e) {
			logger::assert(e.what());
		}
	}
}
constinit uint16_t sys::config::width = 800;
constinit uint16_t sys::config::height = 800 * 3 / 4;
constinit float sys::config::volumeMusic = 0.75f;
constinit float sys::config::volumeSFX = 0.75f;
constinit float sys::config::sensitivity = 1.0f;
constinit SDL_Scancode sys::config::controls[7] = {
	SDL_SCANCODE_UP,
	SDL_SCANCODE_LEFT,
	SDL_SCANCODE_DOWN,
	SDL_SCANCODE_RIGHT,
	SDL_SCANCODE_SPACE,
	SDL_SCANCODE_LSHIFT,
	SDL_Scancode(SDL_NUM_SCANCODES - SDL_BUTTON_LEFT)
};
constinit LANG sys::config::language = LANG::ENG;
constinit bool sys::config::vsync = false;
constinit bool sys::config::ldm = false;
// ! severe boilerplate ahead, proceed with caution
void sys::config::setWidth(uint16_t width) noexcept {
	sys::config::width = width;
	sys::config::height = width * 3 / 4;
}
void sys::config::setVolumeMusic(float volume) noexcept {
	sys::config::volumeMusic = volume;
}
void sys::config::setVolumeSFX(float volume) noexcept {
	sys::config::volumeMusic = volume;
}
void sys::config::setSensitivity(float sensitivity) noexcept {
	sys::config::sensitivity = sensitivity;
}
void sys::config::setControl(CONTROL ctrl, SDL_Scancode scancode) noexcept {
	sys::config::controls[static_cast<int>(ctrl)] = scancode;
}
void sys::config::setLanguage(LANG language) noexcept {
	sys::config::language = language;
}
void sys::config::setVSync(bool vsync) noexcept {
	sys::config::vsync = vsync;
}
void sys::config::setLDM(bool ldm) noexcept {
	sys::config::ldm = ldm;
}
uint16_t sys::config::getWidth(void) noexcept {
	return sys::config::width;
}
const uint16_t sys::config::getHeight(void) noexcept {
	return sys::config::height;
}
float sys::config::getVolumeMusic(void) noexcept {
	return sys::config::volumeMusic;
}
float sys::config::getVolumeSFX(void) noexcept {
	return sys::config::volumeSFX;
}
float sys::config::getSensitivity(void) noexcept {
	return sys::config::sensitivity;
}
SDL_Scancode sys::config::getControl(CONTROL ctrl) noexcept {
	return sys::config::controls[static_cast<int>(ctrl)];
}
LANG sys::config::getLanguage(void) noexcept {
	return sys::config::language;
}
bool sys::config::getVSync(void) noexcept {
	return sys::config::vsync;
}
bool sys::config::getLDM(void) noexcept {
	return sys::config::ldm;
}