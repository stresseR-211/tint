#include "audioStream.hpp"
#include <ranges>
namespace sys {
	namespace audio {
		void init(void) {
			Mix_Init(MIX_INIT_OGG);
			if (int code = Mix_OpenAudio(44100, AUDIO_S16, 2, 2048); code < 0) {
				logger::assert(std::format("Couldn't open audio: {} [{}]", Mix_GetError(), code));
			}
			Mix_AllocateChannels(69); //nice
		}
		void quit(void) {
			Mix_CloseAudio();
			Mix_Quit();
		}
		device::device(CH channel, FX effect, fs::path file) : effect(effect), chtype(channel) {
			switch (channel) {
			case CH::MUS: ch.mus = Mix_LoadMUS(file.c_str()); break;
			case CH::SND: ch.snd = Mix_LoadWAV(file.c_str()); break;
			default: logger::assert("Invalid audio device type. Discarded."); break;
			}
		}
		device::~device(void) {
			switch (chtype) {
			case CH::MUS: Mix_FreeMusic(ch.mus); break;
			case CH::SND:  Mix_FreeChunk(ch.snd); break;
			default: logger::assert("Invalid audio device type. Discarded."); break;
			}
		}
		void device::setEffect(device& dev, FX effect) {
			dev.effect = effect;
		}
		device device::open(fs::path path, CH channel, FX effect) {
			try {
				device dev(channel, effect, path);
				switch (dev.effect) {
				default: logger::assert("Invalid effect type. Defaulted to none."); [[fallthrough]];
				case FX::NONE: break;
				case FX::FILTER: break; //* todo
				}
				if (dev.chtype == CH::MUS) {
					auto file = Mix_LoadMUS(path.c_str());
					if (!file) throw std::runtime_error(std::format("Couldn't load music {}: {}", path.string(), Mix_GetError()));
					dev.ch.mus = file;
				} else if (dev.chtype == CH::SND) {
					auto file = Mix_LoadWAV(path.c_str());
					if (!file) throw std::runtime_error(std::format("Couldn't load sound {}: {}", path.string(), Mix_GetError()));
					dev.ch.snd = file;
				} else {
					logger::assert("Invalid audio device type. Discarded.");
				}
				return dev;
			} catch (const std::exception& e) {
				logger::assert(e.what());
				crash(e.what());
			}
		}
		void device::play(void) {
			play(*this);
		}
		void device::play(const device& dev) {
			switch (dev.chtype) {
			default: logger::assert("Invalid audio device type. Discarded."); return;
			case CH::MUS: if (dev.ch.mus) Mix_PlayMusic(dev.ch.mus, -true); break;
			case CH::SND: if (dev.ch.snd) Mix_PlayChannel(nextFreeChannel(), dev.ch.snd, false); break;
			}
		}
		int device::nextFreeChannel(void) {
			for (int ch : std::ranges::iota_view{0, Mix_AllocateChannels(-1)}) {;
				if (Mix_Playing(ch)) continue;
				return ch;
			}
			return 49; //? we do not care
		}
		void changeVolume(CH channel, unsigned value) {
			if (value > 128) value = 128;
			switch (channel) {
			case CH::MUS: Mix_VolumeMusic(value); break;
			case CH::SND: Mix_Volume(-1, value); break;
			}
		}
	}
}