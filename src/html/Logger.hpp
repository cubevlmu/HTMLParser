// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#pragma once

#include <string>
#include <html/common.hpp>
#include <format>

namespace html {

    enum class LogLevel : u8 {
        kWarning,
        kError,
        kDebug,
        kInfo,
        kTrace
    };

    class Logger final {
    private:
        void emitLog(const std::string& msg, LogLevel level);

    public:
        Logger();
        ~Logger() = default;

        template <typename... Args>
        HP_FORCE_INLINE void info(std::string_view fmtStr, const Args&... args) {
            emitLog(std::vformat(fmtStr, std::make_format_args(args...)), LogLevel::kInfo);
        }
        HP_FORCE_INLINE void info(const char* msg) {
            emitLog(msg, LogLevel::kInfo);
        }

        template <typename... Args>
        HP_FORCE_INLINE void warning(std::string_view fmtStr, const Args&... args) {
            emitLog(std::vformat(fmtStr, std::make_format_args(args...)), LogLevel::kWarning);
        }
        HP_FORCE_INLINE void warning(const char* msg) {
            emitLog(msg, LogLevel::kWarning);
        }

        template <typename... Args>
        HP_FORCE_INLINE void error(std::string_view fmtStr, Args&&... args) {
            emitLog(std::vformat(fmtStr, std::make_format_args(args...)), LogLevel::kError);
        }
        HP_FORCE_INLINE void error(const char* msg) {
            emitLog(msg, LogLevel::kError);
        }

        template <typename... Args>
        HP_FORCE_INLINE void debug(std::string_view fmtStr, const Args&... args) {
#if HP_DEBUG
            emitLog(std::vformat(fmtStr, std::make_format_args(args...)), LogLevel::kDebug);
#else
            (void)fmtStr;
            ((void)args, ...);
#endif
        }
        HP_FORCE_INLINE void debug(const char* msg) {
#if HP_DEBUG
            emitLog(msg, LogLevel::kDebug);
#else
            (void)msg;
#endif
        }

        template <typename... Args>
        HP_FORCE_INLINE void trace(std::string_view fmtStr, const Args&... args) {
#if HP_DEBUG
            emitLog(std::vformat(fmtStr, std::make_format_args(args...)), LogLevel::kTrace);
#else
            (void)fmtStr;
            ((void)args, ...);
#endif
        }
        HP_FORCE_INLINE void trace(const char* msg) {
#if HP_DEBUG
            emitLog(msg, LogLevel::kTrace);
#else
            (void)msg;
#endif
        }
    };

    Logger* getNeoDefaultLogger();
}

#define LogInfo(STR, ...)  ::html::getNeoDefaultLogger()->info(STR, __VA_ARGS__)
#define LogWarn(STR, ...)  ::html::getNeoDefaultLogger()->warning(STR, __VA_ARGS__)
#define LogError(STR, ...) ::html::getNeoDefaultLogger()->error(STR, __VA_ARGS__)
#define LogDebug(STR, ...) ::html::getNeoDefaultLogger()->debug(STR, __VA_ARGS__)
#define LogTrace(STR, ...) ::html::getNeoDefaultLogger()->trace(STR, __VA_ARGS__)