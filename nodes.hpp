#pragma once
#include "preprocess.hpp"
#include "internal.hpp"
#include "core.hpp"
#include <utility>
#include <unordered_map>
typedef enum struct TRIGGER {
	COPY,
	MOVE,
	MOVE_TO,
	ROTATE = 4,
	RESIZE = 8,
	COLMOD = 16,
	LOAD = 32,
	UNLOAD = 64,
	TOGGLE = 128,
	COLLIDE = 256,
	COUNTER = 512,
	TIMER = 1024,
	TEXTURE_ONLY = 2048,
	HITBOX_ONLY = 4096
} TRIGGER;
typedef enum struct COLMOD {
	NORMAL,
	ADDITIVE,
	MULTIPLY,
	OVERLAY,
	HARDMIX,
	INVERT
} COLMOD;
typedef enum struct BEZIER {
	CUSTOM,
	STRAIGHT,
	FADE,
	FADE_OUT,
	FADE_IN,
	ELASTIQUE,
	ELASTIQUE_OUT,
	ELASTIQUE_IN,
	BACK,
	BACK_OUT,
	BACK_IN,
	ZIGZAG
} BEZIER;
namespace node {
	class window;
	class object;
	namespace create {
		window EXPORT window(int width, int height) noexcept;
		object EXPORT object(fs::path texture, SDL_Rect bounds = {}, SDL_Rect hitbox = {}) noexcept;
	}
	struct bezierCurve {
		float x1, y1, x2, y2;
		template <typename... T> bezierCurve(T...) = delete;
		template <typename... T> bezierCurve operator=(T...) = delete;
		bezierCurve(void) = delete;
		bezierCurve(const bezierCurve&) = default;
		bezierCurve& operator=(const bezierCurve&) = default;
		~bezierCurve(void) noexcept = default;
		explicit bezierCurve(float, float, float, float);
	};
	inline const std::unordered_map<const BEZIER, const node::bezierCurve> bezierCurves {
		std::make_pair(BEZIER::STRAIGHT, node::bezierCurve(0.0f, 0.0f, 1.0f, 1.0f)),
		std::make_pair(BEZIER::FADE, node::bezierCurve(0.5f, 0.0f, 0.5f, 1.0f)),
		std::make_pair(BEZIER::FADE_OUT, node::bezierCurve(0.0f, 0.0f, 0.5f, 1.0f)),
		std::make_pair(BEZIER::FADE_IN, node::bezierCurve(0.0f, 0.5f, 1.0f, 1.0f)),
		std::make_pair(BEZIER::ELASTIQUE, node::bezierCurve(0.75f, 0.0f, 0.25f, 1.0f)),
		std::make_pair(BEZIER::ELASTIQUE_OUT, node::bezierCurve(0.25f, 1.0f, 0.5f, 1.0f)),
		std::make_pair(BEZIER::ELASTIQUE_IN, node::bezierCurve(0.5f, 0.0f, 0.75f, 0.0f)),
		std::make_pair(BEZIER::BACK, node::bezierCurve(0.5f, -0.5f, 0.5f, 1.5f)),
		std::make_pair(BEZIER::BACK_OUT, node::bezierCurve(0.5f, 0.0f, 0.5f, 1.5f)),
		std::make_pair(BEZIER::BACK_IN, node::bezierCurve(0.5f, -0.5f, 0.5f, 1.0f)),
		std::make_pair(BEZIER::ZIGZAG, node::bezierCurve(0.5f, 1.25f, 0.5f, -0.25f)),
	};
	class EXPORT object {
	protected:
		std::string path;
		SDL_Surface* texture;
		SDL_Rect bounds;
		[[maybe_unused]] SDL_Rect hitbox;
		uint64_t index;
		explicit object(fs::path texture, SDL_Rect, SDL_Rect);
	public:
		object copy(void);
		void move(int x, int y, bezierCurve) noexcept;
		void moveAbs(int x, int y, bezierCurve) noexcept;
		void rotate(float d) noexcept;
		void scale(uint8_t z) noexcept;
		void colmod(COLMOD) noexcept;
		void colmod(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
		void colmod(uint8_t h, uint8_t s, uint8_t v) noexcept;
		void load(void) noexcept;
		void unload(void) noexcept;
		void toggle(void) noexcept;
		void link(TRIGGER) noexcept;
		template <typename... T> object(T...) = delete;
		template <typename... T> object operator=(T...) = delete;
		object(void) = delete;
		object(const object&) = delete;
		object operator=(const object&); //  todo?
		~object(void) noexcept;
		friend object EXPORT create::object(fs::path texture, SDL_Rect bounds, SDL_Rect hitbox) noexcept;
	};
	class EXPORT window final {
		SDL_Window* self;
		SDL_Renderer* renderer;
		explicit window(int, int) noexcept;
	public:
		void update(void) noexcept;
		void resize(void) noexcept;
		void show(void) noexcept;
		void hide(void) noexcept;
		void close(void) noexcept;
		template <typename... T> window(T...) = delete;
		template <typename... T> window operator=(T...) = delete;
		window(void) = delete;
		window(const window&) = delete;
		window operator=(const window&) = delete;
		~window(void) noexcept;
		friend window EXPORT node::create::window(int width, int height) noexcept;
	};
}