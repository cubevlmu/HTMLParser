// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

// This project contains code from [https://github.com/rangerlee/htmlparser], licensed under Apache License 2.0.
// Modifications by cubevlmu are licensed under MIT License.

#pragma once

#include <html/common.hpp>

#include <vector>
#include <memory>
#include <unordered_map>
#include <set>

namespace html {

    class HElement : public std::enable_shared_from_this<HElement> {
    public:
        friend class HParser;

    public:
        typedef std::vector<HValue>::const_iterator ChildIterator;
        typedef std::unordered_map<std::string, std::string>::const_iterator AttributeIterator;

        ChildIterator childBegin();
        ChildIterator childEnd();

        AttributeIterator attributeBegin();
        AttributeIterator attributeEnd();

    public:
        HElement() = default;
        HElement(HValue p)
                : m_parent(p) {}

        std::string getAttribute(const std::string &k);

        HValue getElementById(const std::string &id);
        std::vector<HValue> getElementByClassName(const std::string &name);
        std::vector<HValue> getElementByTagName(const std::string &name);

        void selectElement(const std::string &rule, std::vector<HValue> &result);

        HValue getParent();

        const std::string& getValue();
        const std::string& getName();

        std::string toPlainText() const;
        std::string toHtml() const;

        void asPlainText(std::string &str) const;
        void asHtml(std::string &str) const;

    private:
        void getElementByClassName(const std::string &name, std::vector<HValue> &result);
        void getElementByTagName(const std::string &name, std::vector<HValue> &result);
        void getAllElement(std::vector<HValue> &result);
        void parse(const std::string &attr);

        static std::set<std::string>splitClassName(const std::string &name);
        static void insertIfNotExists(std::vector<HValue> &vec, const HValue &ele);

    private:
        std::string m_name;
        std::string m_value;
        std::unordered_map<std::string, std::string> m_attribute;
        std::weak_ptr<HElement> m_parent;
        std::vector<HValue> m_children;
    };

}