// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#include "Logger.hpp"

#include <memory>

namespace html {

    void Logger::emitLog(const std::string& msg, LogLevel level) {
        switch(level)
        {
            case LogLevel::kWarning:
                printf("[Warn] %s\n", msg.c_str());
                break;
            case LogLevel::kError:
                printf("[Error] %s\n", msg.c_str());
                break;
            case LogLevel::kDebug:
                printf("[Debug] %s\n", msg.c_str());
                break;
            case LogLevel::kInfo:
                printf("[Info] %s\n", msg.c_str());
                break;
            case LogLevel::kTrace:
                printf("[Trace] %s\n", msg.c_str());
                break;
        }
    }

    Logger::Logger()
    {
    }


    Logger* getNeoDefaultLogger() {
        static Logger logger{};
        return &logger;
    }

}