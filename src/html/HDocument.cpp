// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

// This project contains code from [https://github.com/rangerlee/htmlparser], licensed under Apache License 2.0.
// Modifications by cubevlmu are licensed under MIT License.

#include "HDocument.hpp"

#include "HElement.hpp"

namespace html {

    HValue HDocument::getElementById(const std::string& id)
    {
        return m_root ? m_root->getElementById(id) : nullptr;
    }

    std::vector<HValue> HDocument::getElementByClassName(const std::string& name)
    {
        return m_root ? m_root->getElementByClassName(name) : std::vector<HValue>{};
    }

    std::vector<HValue> HDocument::getElementByTagName(const std::string& name)
    {
        return m_root ? m_root->getElementByTagName(name) : std::vector<HValue>{};
    }

    std::vector<HValue> HDocument::selectElement(const std::string& rule)
    {
        std::vector<HValue> result;
        if (m_root) {
            // 递归从 root 开始，而不是只遍历 children
            m_root->selectElement(rule, result);
        }
        return result;
    }

    std::string HDocument::toHtml() const
    {
        return m_root ? m_root->toHtml() : "";
    }

    std::string HDocument::toPlainText() const
    {
        return m_root ? m_root->toPlainText() : "";
    }

}