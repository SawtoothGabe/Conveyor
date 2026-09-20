#pragma once

#include <Common/Platform.hpp>
#include <IO/Logger.hpp>

#ifndef NDEBUG
#ifdef CONV_PLATFORM_WINDOWS
#define CONV_DEBUGBREAK() __debugbreak()
#elif defined(CONV_PLATFORM_MACOS) || defined(CONV_PLATFORM_LINUX)
#include <signal.h>
#define CONV_DEBUGBREAK() raise(SIGTRAP)
#endif

#define CONV_ASSERT(check, ...) do { if (!(check)) { LOG_ERROR_TRACE(__VA_ARGS__); CONV_DEBUGBREAK(); } } while(0)
#else
#define CONV_DEBUGBREAK() ;
#define CONV_ASSERT(...) ;
#endif