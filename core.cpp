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
		}
		if (int code = TTF_Init(); code < 0) {
			logger::assert(std::format("Couldn't initialise SDL_TTF: {} [{}]", TTF_GetError(), code));
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
constinit uint16_t config::width = 800;
constinit uint16_t config::height = 800 * config::resolution.den / config::resolution.num;
constinit float config::volumeMUS = 0.75f;
constinit float config::volumeSFX = 0.75f;
constinit float config::sensitivity = 1.0f;
constinit SDL_Scancode config::controls[7] = {
	SDL_SCANCODE_W,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_SPACE,
	SDL_SCANCODE_LSHIFT,
	SDL_Scancode(SDL_NUM_SCANCODES - SDL_BUTTON_LEFT)
};
constinit LANG config::language = LANG::ENG;
constinit bool config::vsync = false; // ! set to true on release
constinit bool config::ldm = false;
std::any config::get(CFG option) noexcept {
	switch (option) {
	case CFG::RESO: return resolution;
	case CFG::WIDTH: return width;
	case CFG::HEIGHT: return height;
	case CFG::VOLMUS: return volumeMUS;
	case CFG::VOLSFX: return volumeSFX;
	case CFG::SENS: return sensitivity;
	case CFG::LANG: return language;
	case CFG::VSYNC: return vsync;
	case CFG::LDM: return ldm;
	[[unlikely]] default: return nullptr;
	};
}
SDL_Scancode config::get(CTRL control) noexcept {
	return controls[static_cast<int>(control)];
}
void config::set(CFG option, std::any value) {
	std::string errstr;
	try {
		switch (option) {
		case CFG::RESO:
			sys::logger::assert("The aspect ratio is locked to 4:3 (discarded)");
			break;
		case CFG::WIDTH:
			errstr = "Config parameter `width` must be a number (discarded)";
			width = std::any_cast<uint16_t>(value);
			height = width * config::resolution.den / config::resolution.num;
			break;
		case CFG::HEIGHT:
			sys::logger::assert("Config parameter `height` must not be changed manually (discarded)");
			break;
		case CFG::VOLMUS:
			errstr = "Config parameter `volumeMUS` must be of type `float` (discarded)";
			volumeMUS = std::any_cast<float>(value);
			break;
		case CFG::VOLSFX:
			errstr = "Config parameter `volumeSFX` must be of type `float` (discarded)";
			volumeSFX = std::any_cast<float>(value);
			break;
		case CFG::SENS:
			errstr = "Config parameter `sensitivity` must be of type `float` (discarded)";
			sensitivity = std::any_cast<float>(value);
			break;
		case CFG::LANG:
			errstr = "Config parameter `language` must be an enum (discarded)";
			language = std::any_cast<LANG>(value);
			break;
		case CFG::VSYNC:
			errstr = "Config parameter `vsync` must be a boolean (discarded)";
			vsync = std::any_cast<bool>(value);
			break;
		case CFG::LDM:
			errstr = "Config parameter `ldm` must be a boolean (discarded)";
			ldm = std::any_cast<bool>(value);
			break;
		[[unlikely]] default: break;
		};
	} catch (const std::bad_any_cast& e) {
		sys::logger::assert(std::format("{} [{}]", errstr, e.what()));
	}
}
void config::set(CTRL control, SDL_Scancode value) noexcept {
	controls[static_cast<int>(control)] = value;
}