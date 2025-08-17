// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#pragma once

#define _HP_STR_HELPER(x) #x
#define _HP_STR(x) _HP_STR_HELPER(x)

#define HP_SAFE_ALIGNMENT 16
#define HP_CACHE_LIHP_SIZE 64

#if defined(__clang__)
#	define HP_COMPILER_CLANG 1
#	define HP_COMPILER_GCC 0
#	define HP_COMPILER_MSVC 0
#	define HP_COMPILER_GCC_COMPATIBLE 1
#	define HP_COMPILER_STR "clang " __VERSION__
#elif defined(__GNUC__) || defined(__GNUG__)
#	define HP_COMPILER_CLANG 0
#	define HP_COMPILER_GCC 1
#	define HP_COMPILER_MSVC 0
#	define HP_COMPILER_GCC_COMPATIBLE 1
#	define HP_COMPILER_STR "gcc " __VERSION__
#elif defined(_MSC_VER)
#	define HP_COMPILER_CLANG 0
#	define HP_COMPILER_GCC 0
#	define HP_COMPILER_MSVC 1
#	define HP_COMPILER_GCC_COMPATIBLE 0
#	define HP_COMPILER_STR "MSVC " _HP_STR(_MSC_FULL_VER)
#else
#	error Unknown compiler
#endif

// Operating system
#if defined(__WIN32__) || defined(_WIN32)
#	define HP_LINUX 0
#	define HP_ANDROID 0
#	define HP_MACOS 0
#	define HP_IOS 0
#	define HP_WINDOWS 1
#	define HP_STR "Windows"
#elif defined(__APPLE_CC__)
#	if (defined(__ENVIRONMENT_IPHOHP_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHOHP_OS_VERSION_MIN_REQUIRED__ >= 40000) || (defined(__IPHOHP_OS_VERSION_MIN_REQUIRED) && __IPHOHP_OS_VERSION_MIN_REQUIRED >= 40000)
#		define HP_LINUX 0
#		define HP_ANDROID 0
#		define HP_MACOS 0
#		define HP_IOS 1
#		define HP_WINDOWS 0
#		define HP_STR "iOS"
#	else
#		define HP_LINUX 0
#		define HP_ANDROID 0
#		define HP_MACOS 1
#		define HP_IOS 0
#		define HP_WINDOWS 0
#		define HP_STR "MacOS"
#	endif
#elif defined(__ANDROID__)
#	define HP_LINUX 0
#	define HP_ANDROID 1
#	define HP_MACOS 0
#	define HP_IOS 0
#	define HP_WINDOWS 0
#	define HP_STR "Android"
#elif defined(__linux__)
#	define HP_LINUX 1
#	define HP_ANDROID 0
#	define HP_MACOS 0
#	define HP_IOS 0
#	define HP_WINDOWS 0
#	define HP_STR "Linux"
#else
#	error Unknown OS
#endif


#if HP_LINUX || HP_ANDROID || HP_MACOS || HP_IOS
#	define HP_POSIX 1
#else
#	define HP_POSIX 0
#endif


#if HP_COMPILER_GCC_COMPATIBLE
#	if defined(__arm__) || defined(__aarch64__)
#		define HP_CPU_ARCH_X86 0
#		define HP_CPU_ARCH_ARM 1
#	elif defined(__i386__) || defined(__amd64__)
#		define HP_CPU_ARCH_X86 1
#		define HP_CPU_ARCH_ARM 0
#	endif
#elif HP_COMPILER_MSVC
#	if defined(_M_ARM)
#		define HP_CPU_ARCH_X86 0
#		define HP_CPU_ARCH_ARM 1
#	elif defined(_M_X64) || defined(_M_IX86)
#		define HP_CPU_ARCH_X86 1
#		define HP_CPU_ARCH_ARM 0
#	endif
#endif


#if defined(HP_CPU_ARCH_X86) && HP_CPU_ARCH_X86
#	define HP_GPU_ARCH_STR "x86"
#elif defined(HP_CPU_ARCH_ARM) && HP_CPU_ARCH_ARM
#	define HP_GPU_ARCH_STR "arm"
#else
#	error Unknown CPU arch
#endif


#if !HP_ENABLE_SIMD
#	define HP_SIMD_NONE 1
#	define HP_SIMD_SSE 0
#	define HP_SIMD_NEON 0
#	define HP_SIMD_STR "None"
#elif HP_CPU_ARCH_X86
#	define HP_SIMD_NONE 0
#	define HP_SIMD_SSE 1
#	define HP_SIMD_NEON 0
#	define HP_SIMD_STR "SSE"
#else
#	define HP_SIMD_NONE 0
#	define HP_SIMD_SSE 0
#	define HP_SIMD_NEON 1
#	define HP_SIMD_STR "Neon"
#endif


#if HP_COMPILER_GCC_COMPATIBLE
#	define HP_RESTRICT __restrict
#	define HP_FORCE_INLINE inline __attribute__((always_inline))
#	define HP_DONT_INLINE __attribute__((noinline))
#	define HP_UNUSED __attribute__((__unused__))
#	define HP_COLD __attribute__((cold, optimize("Os")))
#	define HP_HOT __attribute__ ((hot))
#	define HP_UNREACHABLE() __builtin_unreachable()
#	define HP_PREFETCH_MEMORY(addr) __builtin_prefetch(addr)
#	define HP_CHECK_FORMAT(fmtArgIdx, firstArgIdx) __attribute__((format(printf, fmtArgIdx + 1, firstArgIdx + 1))) // On methods you need to include "this"
#	define HP_PURE __attribute__((pure))
#elif HP_COMPILER_MSVC
#	define HP_RESTRICT
#	define HP_FORCE_INLINE __forceinline
#	define HP_DONT_INLINE
#	define HP_UNUSED
#	define HP_COLD
#	define HP_HOT
#	define HP_UNREACHABLE() __assume(false)
#	define HP_PREFETCH_MEMORY(addr) (void)(addr)
#	define HP_CHECK_FORMAT(fmtArgIdx, firstArgIdx)
#	define HP_PURE
#else
#	define HP_RESTRICT
#	define HP_FORCE_INLINE
#	define HP_DONT_INLINE
#	define HP_UNUSED
#	define HP_COLD
#	define HP_HOT
#	define HP_UNREACHABLE()
#	define HP_PREFETCH_MEMORY(addr) (void)(addr)
#	define HP_CHECK_FORMAT(fmtArgIdx, firstArgIdx)
#	define HP_PURE
#endif


#if HP_POSIX
#define HP_API __attribute__((visibility ("default")))
#elif HP_WINDOWS
#define HP_API __declspec(dllexport)
#else
#define HP_API
#endif
