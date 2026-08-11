#pragma once
#include "preprocess.hpp"
#include "internal.hpp"
#include <SDL2/SDL_mixer.h>
#include <vector>

typedef enum struct CH {
	MUS,
	SND
} CH;
typedef enum struct FX {
	NONE,
	FILTER
} FX;
namespace sys {
	namespace audio {
		union chswitch {
			Mix_Music* mus;
			Mix_Chunk* snd;
		};
		class EXPORT device {
			chswitch ch;
			FX effect;
			CH chtype;
			[[nodiscard]] explicit device(CH channel, FX effect, fs::path file);
		public:
			template <typename... T> device(T...) = delete;
			template <typename... T> device operator=(T...) = delete;
			device(void) = delete;
			device(const device&) = default;
			device& operator=(const device&) = delete;
			~device(void);
			static void setEffect(device&, FX);
			static device open(fs::path, CH, FX = FX::NONE);
			void play(void);
			static void play(const device&);
			void stop(void);
			static void stop(const device&);
			static void close(const device&);
			static int nextFreeChannel(void);
			friend void changeVolume(unsigned value, CH channel);
		};
		void EXPORT init(void);
		void EXPORT quit(void);
		void changeVolume(CH channel, unsigned value);
	}
}