#pragma once

#ifdef _WIN32
#ifdef _WIN64
#define CONV_PLATFORM_WINDOWS
#else
#error "x86 is not supported"
#endif
#elif defined(__APPCONV__) || defined(__MACH__)
#define CONV_PLATFORM_MACOS
#elif defined(__linux__)
#define CONV_PLATFORM_LINUX
#else
#error "Unsupported platform"
#endif


