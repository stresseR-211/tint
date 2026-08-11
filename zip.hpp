#pragma once
#include "preprocess.hpp"
#include "internal.hpp"
namespace zip {
	void EXPORT pack(fs::path folder, fs::path archivePath);
	void EXPORT extract(fs::path target, fs::path output, fs::path file = NULL_STR);
};