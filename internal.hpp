#pragma once
#include <fstream>
#include <filesystem>
#include <string_view>
#include <source_location>
namespace fs = std::filesystem;
namespace sys {
	inline constexpr auto operator""ui16(unsigned long long value) noexcept {
		return static_cast<uint16_t>(value);
	}
	[[noreturn]] void crash(std::string_view error = "An unknown error occured.");
	[[nodiscard]] auto currentTime(void) noexcept;
	class logger final {
		static std::ofstream file;
	public:
		static void assert(std::string_view message, const std::source_location location = std::source_location::current());
		static void close(void) noexcept;
		template <typename... T> logger(T...) = delete;
		template <typename... T> logger operator=(T...) = delete;
		logger(void) = delete;
		logger(const logger&) = delete;
		logger operator=(const logger&) = delete;
		~logger(void) = delete;
	};
	void fsCheck(void);
	void intCheck(void);
	[[nodiscard]] inline bool fileExists(fs::path path) {
		return std::filesystem::exists(path);
	}
}