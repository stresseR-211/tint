#include "zip.hpp"
#include <algorithm>
#include <vector>
const char etx{'\u0003'}, eom{'\u0019'};
void zip::pack(fs::path folder, fs::path archivePath) {
	try {
		if (!sys::fileExists(folder)) throw std::runtime_error("Target file doesn't exist.");
		if (!fs::is_directory(folder)) throw std::runtime_error("Target file isn't a directory.");
		std::ofstream archive(archivePath, std::ios::trunc | std::ios::binary);
		if (!archive) throw std::runtime_error(std::format("Couldn't open {} for writing.", archivePath));
		uint_least32_t offset{0};
		std::vector<fs::directory_entry> files;
		for (const auto& entry : fs::recursive_directory_iterator(folder)) {
			if (entry.is_regular_file()) files.push_back(entry);
		}
		archive << ".quarc_";
		for (const auto& entry : files) {
			archive << std::format("\"{}\"{}|{}\n", entry.path().lexically_relative(folder), offset, entry.file_size());
			offset += static_cast<uint_least32_t>(entry.file_size());
		}
		archive << etx << '\n';
		for (const auto& entry : files) {
			std::ifstream file(entry.path(), std::ios::binary);
			if (!file) throw std::runtime_error(std::format("Couldn't open {} for reading.", entry.path()));
			archive << file.rdbuf();
		}
		archive << eom;
		archive.close();
	} catch (const std::exception& e) {
		sys::logger::assert(e.what());
		sys::crash(e.what());
	}
}
void zip::extract(fs::path target, fs::path output, fs::path file) {
	try {
		if (!sys::fileExists(target)) throw std::runtime_error("Target file doesn't exist.");
		if (!sys::fileExists(output)) throw std::runtime_error("Output directory doesn't exist.");
		if (!fs::is_directory(output)) throw std::runtime_error("Target file isn't a directory.");
		std::ifstream archive(target, std::ios::binary);
		if (!archive) throw std::runtime_error(std::format("Couldn't open {} for reading.", target.string()));
		char header[7]{};
		archive.read(header, 7);
		if (std::string_view(header, 7) != ".quarc_") throw std::runtime_error("Target file isn't an archive.");
		struct entry {
			fs::path path;
			uint_least32_t offset;
			uint_least32_t size;
		};
		std::vector<entry> entries;
		std::string line;
		while (std::getline(archive, line)) {
			if (line.empty()) continue;
			if (line[0] == etx) break;
			if (line.front() != '"') throw std::runtime_error("Malformed index entry (missing opening quote).");
			size_t closeQuote = line.find('"', 1);
			if (closeQuote == std::string::npos) throw std::runtime_error("Malformed index entry (missing enclosing quote).");
			std::string relPath = line.substr(1, closeQuote - 1);
			std::string rest = line.substr(closeQuote + 1);
			uint_least32_t offset{0}, size{0};
			if (std::sscanf(rest.c_str(), "%u|%u", &offset, &size) != 2) throw std::runtime_error(std::format("Malformed index entry: {}", line));
			entries.push_back({std::move(relPath), offset, size});
		}
		const auto startOfData = archive.tellg();
		const bool extractAll = file == NULL_STR;
		if (!extractAll) file = file.lexically_normal();
		bool found{false};
		for (const auto& entry : entries) {
			if (!extractAll && entry.path.lexically_normal() != file) continue;
			found = true;
			fs::path outPath = output / entry.path;
			fs::create_directories(outPath.parent_path());
			archive.seekg(startOfData + static_cast<std::streamoff>(entry.offset));
			std::ofstream outFile(outPath, std::ios::binary | std::ios::trunc);
			if (!outFile) throw std::runtime_error(std::format("Couldn't create output file: {}", outPath.string()));
			constexpr std::size_t bufferSize = 64 * 1024;
			std::vector<char> buffer(bufferSize);
			std::uint64_t remaining = entry.size;
			while (remaining > 0) {
				const std::size_t read = static_cast<std::size_t>(std::min<std::uint64_t>(remaining, buffer.size()));
				archive.read(buffer.data(), static_cast<std::streamsize>(read));
				if (archive.gcount() != static_cast<std::streamsize>(read)) {
					throw std::runtime_error(std::format("Unexpected end of archive: {}", entry.path.string()));
				}
				outFile.write(buffer.data(), static_cast<std::streamsize>(read));
				if (!outFile)  throw std::runtime_error(std::format("Failed writing output file: {}", outPath.string()));
				remaining -= read;
			}
			if (!extractAll) break;
		}
		if (!extractAll && !found) throw std::runtime_error(std::format("File not present in archive: {}", file.string()));
	} catch (const std::exception& e) {
		sys::logger::assert(e.what());
		sys::crash(e.what());
	}
}