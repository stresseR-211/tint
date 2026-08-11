#include "internal.hpp"
#include <cstdio>
#include <chrono>
#include <utility>
void sys::crash(std::string_view error) {
	std::fwrite(std::format("[TINT] {}", error).c_str(), error.length() + 7, 1, stderr);
	std::abort();
}
auto sys::currentTime(void) noexcept {
	return std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
}
std::ofstream sys::logger::file;
void sys::logger::assert(std::string_view message, const std::source_location source) {
	if (!file.is_open()) {
		file.open("../debug.log", std::ios::app);
		if (file.fail()) crash("Failed to open debug.log: failed to open the file stream.");
	}
	file << std::format("[{}] {}: {}\n", currentTime(), source.function_name(), message);
	file.close();
}
void sys::logger::close(void) noexcept {
	file.close();
}
void sys::fsCheck(void) {
	uintmax_t (*megabytes)(uintmax_t) = [](uintmax_t bytes) noexcept {
		return bytes / std::mega::num;
	};
	void (*chkdsk)(const fs::directory_entry&) = [](const fs::directory_entry& entry) {
		if (entry.is_directory()) {
			fs::path dummyPath = entry.path() / ".dummy";
			std::ofstream dummy(dummyPath, std::ios::out | std::ios::trunc);
			if (!dummy) crash("I/O error: read-only filesystem?");
			dummy.close();
			fs::remove(dummyPath);
		} else {
			std::ifstream dummy(entry.path(), std::ios::in);
			if (!dummy) crash("I/O error: can't access the game files.");
			dummy.close();
		}
	};
	if (std::cmp_less(megabytes(fs::space("../").available), 25)) crash("Insufficient free disk space (minimum 25MB is required).");
	try {
		for (const fs::directory_entry& entry : fs::recursive_directory_iterator("../data")) {
			chkdsk(entry);
		}
		for (const fs::directory_entry& entry : fs::recursive_directory_iterator("../locale")) {
			chkdsk(entry);
		}
	} catch (const fs::filesystem_error& e) {
		crash(std::format("Couldn't perform the filesystem check: {}", e.what()));
	}
}
void sys::intCheck(void) {
	try {
		if (!fs::exists("../data")) crash("Resource directory doesn't exist.");
		if (!fs::exists("../data/embed")) crash("Script directory doesn't exist.");
		if (!fs::exists("../data/temp")) fs::create_directory("../data/temp");
	} catch (const fs::filesystem_error& e) {
		crash(std::format("Couldn't perform the integrity check: {}", e.what()));
	}
}