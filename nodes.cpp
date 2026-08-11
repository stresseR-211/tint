#include "nodes.hpp"
#include <SDL2/SDL_image.h>
	// * todo: refactoring
namespace node {
	bezierCurve::bezierCurve(float x1, float y1, float x2, float y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}
	window::window(int width, int height) noexcept {
		self = SDL_CreateWindow(NULL_STR, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
		if (!self) {
			sys::logger::assert(SDL_GetError());
			sys::crash(SDL_GetError());
		} 
		renderer = SDL_CreateRenderer(self, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			sys::logger::assert(SDL_GetError());
			sys::crash(SDL_GetError());
		} 
		update();
	}
	window::~window(void) noexcept {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(self);
	}
	void window::update(void) noexcept {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
	void window::resize(void) noexcept {}
	void window::show(void) noexcept {
		SDL_ShowWindow(self);
		update();
	}
	void window::hide(void) noexcept {
		SDL_HideWindow(self);
	}
	object::object(fs::path texture, SDL_Rect, SDL_Rect) {
		try {
			if (!sys::fileExists(texture)) throw std::runtime_error("The file doesn't exist.");
			this->texture = IMG_Load(texture.relative_path().string().c_str());
		} catch (const std::exception& e) {
			sys::logger::assert(std::format("Couldn't load the surface texture: {}", e.what()));
			sys::crash(e.what());
		}
	}
	namespace create {
		class window window(int width, int height) noexcept {
			return node::window(width, height);
		}
		class object object(fs::path texture, SDL_Rect bounds , SDL_Rect hitbox) noexcept {
			return node::object(texture, bounds, hitbox);
		}
	}
}