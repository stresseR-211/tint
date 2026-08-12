#pragma once
#include "preprocess.hpp"
#include <SDL2/SDL.h>
typedef enum struct LANG {
	ENG, DEU, RUS, UKR,
	ESP, PTG, ROM, JPN
} LANG;
typedef enum struct CONTROL {
	UP,
	LEFT,
	DOWN,
	RIGHT,
	DASH,
	GRAZE,
	FIRE
} CONTROL;
typedef enum struct VOLUME {
	MUSIC,
	SFX
} VOLUME;
namespace sys {
	void EXPORT init(void);
	void EXPORT quit(void);
	class EXPORT config final {
		static constinit uint16_t width;
		static constinit uint16_t height;
		static constinit float volumeMusic;
		static constinit float volumeSFX;
		static constinit float sensitivity;
		static constinit SDL_Scancode controls[7];
		static constinit LANG language;
		static constinit bool vsync;
		static constinit bool ldm;
	public: // * fuck it
		static void setWidth(uint16_t) noexcept;
		static void setVolumeMusic(float) noexcept;
		static void setVolumeSFX(float) noexcept;
		static void setSensitivity(float) noexcept;
		static void setControl(CONTROL, SDL_Scancode) noexcept;
		static void setLanguage(LANG) noexcept;
		static void setVSync(bool) noexcept;
		static void setLDM(bool) noexcept;
		static uint16_t getWidth(void) noexcept;
		static const uint16_t getHeight(void) noexcept;
		static float getVolumeMusic(void) noexcept;
		static float getVolumeSFX(void) noexcept;
		static float getSensitivity(void) noexcept;
		static SDL_Scancode getControl(CONTROL) noexcept;
		static LANG getLanguage(void) noexcept;
		static bool getVSync(void) noexcept;
		static bool getLDM(void) noexcept;
		template <typename T> config(T) = delete;
		template <typename T> config operator=(T) = delete;
		config(void) = delete;
		config(const config&) = delete;
		config operator=(const config&) = delete;
		~config(void) = delete;
	};
}