#pragma once
#include "preprocess.hpp"
#include <ratio>
#include <any>
#include <SDL2/SDL.h>
namespace sys {
	void EXPORT init(void);
	void EXPORT quit(void);
}
typedef enum struct LANG {
	ENG,
	ROM,
	RUS,
	UKR,
	PTG,
	JPN
} LANG;
typedef enum struct CFG {
	RESO,
	WIDTH,
	HEIGHT,
	VOLMUS,
	VOLSFX,
	SENS,
	LANG,
	VSYNC,
	LDM
} CFG;
typedef enum struct CTRL {
	UP,
	LEFT,
	DOWN,
	RIGHT,
	DASH,
	GRAZE,
	FIRE
} CTRL;

// * todo: refactor everything, this is shit
class EXPORT config final {
	static constexpr std::ratio<4,3> resolution{}; // * read only
	static constinit uint16_t width;
	static constinit uint16_t height;  // * auto-adjusted
	static constinit float volumeMUS;
	static constinit float volumeSFX;
	static constinit float sensitivity;
	static constinit SDL_Scancode controls[7];
	static constinit LANG language;
	static constinit bool vsync;
	static constinit bool ldm; // * low detail mode
public:
	static std::any get(CFG option) noexcept;
	static SDL_Scancode get(CTRL control) noexcept;
	static void set(CFG option, std::any value);
	static void set(CTRL control, SDL_Scancode value) noexcept;
	template <typename T> config(T) = delete;
	template <typename T> config operator=(T) = delete;
	config(void) = delete;
	config(const config&) = delete;
	config operator=(const config&) = delete;
	~config(void) = delete;
};