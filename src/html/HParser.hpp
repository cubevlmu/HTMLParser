// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

// This project contains code from [https://github.com/rangerlee/htmlparser], licensed under Apache License 2.0.
// Modifications by cubevlmu are licensed under MIT License.

#pragma once

#include <html/common.hpp>

#include <vector>
#include <unordered_map>
#include <memory>
#include <set>

namespace html {

    class HParser {
    public:
        HParser();

        HDoc parse(const char *data, psize len);
        HP_FORCE_INLINE HDoc parse(const std::string &data) {
            return parse(data.data(), data.size());
        }

    private:
        psize parseElement(psize index, HValue &element);
        psize skipUntil(psize index, const char *data);
        psize skipUntil(psize index, const char data);

    private:
        const char* m_stream;
        psize m_length;
        std::set<std::string> m_self_closing_tags;
        HValue m_root;
    };

}