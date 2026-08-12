/* --- Do not change anything --- */
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef __cplusplus
#error --- C++ is required ---
#elif __cplusplus < 202400L
#error --- Go upgrade idk ---
#else
#if defined(_MSC_VER)
#define libexport __declspec(dllexport)
#define libimport __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#define libexport __attribute__((dllexport))
#define libimport __attribute__((dllimport))
#else
#error --- Unsupported compiler i suppose ---
#endif /* compiler support */
#ifdef _WIN32 
#ifndef GAME
#define EXPORT libexport
#else
#define EXPORT libimport
#endif /* GAME */
#else
#define EXPORT
#endif /* _WIN32 */
#include <cstdint>
#include <string>
#include <type_traits>
#define __concat(x, y) x##y
#define __concatExpanded(x, y) __concat(x, y)
#define __uniqueName(x) __concatExpanded(x, __COUNTER__)
#define watchdog(lambda) auto __uniqueName(__scopeGuard) = __makeScopeGuard(lambda)
template <typename T> class EXPORT __scopeGuard final {
	T lambda;
	__scopeGuard(T func) noexcept : lambda(func) {};
public:
	template <typename U> friend constexpr std::enable_if_t<std::is_invocable<U>::value, __scopeGuard<U>> __makeScopeGuard(U func) noexcept;
	template <typename... U> __scopeGuard(U...) = delete;
	template <typename... U> __scopeGuard operator=(U...) = delete;
	__scopeGuard(void) = delete;
	__scopeGuard(const __scopeGuard&) = delete;
	__scopeGuard operator=(const __scopeGuard&) = delete;
	~__scopeGuard(void) noexcept {
		lambda();
	};
};
template <typename  T> constexpr std::enable_if_t<std::is_invocable<T>::value, __scopeGuard<T>> EXPORT __makeScopeGuard(T func) noexcept {
	return __scopeGuard<T>(std::move(func));
}
inline constexpr const char* NULL_STR = "";
#endif /*__cplusplus */