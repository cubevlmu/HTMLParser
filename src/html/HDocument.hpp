// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

// This project contains code from [https://github.com/rangerlee/htmlparser], licensed under Apache License 2.0.
// Modifications by cubevlmu are licensed under MIT License.

#pragma once

#include <html/common.hpp>

#include <vector>

namespace html {

    class HDocument {
    public:
        HDocument(HValue &root)
                : m_root(root) {}

        HValue getElementById(const std::string &id);

        std::vector<HValue> getElementByClassName(const std::string &name);
        std::vector<HValue> getElementByTagName(const std::string &name);
        std::vector<HValue> selectElement(const std::string &rule);

        std::string toHtml() const;
        std::string toPlainText() const;

    private:
        HValue m_root;
    };

}