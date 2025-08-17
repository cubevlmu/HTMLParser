// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

// This project contains code from [https://github.com/rangerlee/htmlparser], licensed under Apache License 2.0.
// Modifications by cubevlmu are licensed under MIT License.

#include "HElement.hpp"

#include <cctype>
#include <string>
#include <unordered_map>
#include "Logger.hpp"

namespace html {

    HElement::ChildIterator HElement::childBegin()
    {
        return m_children.begin();
    }

    HElement::ChildIterator HElement::childEnd()
    {
        return m_children.end();
    }

    HElement::AttributeIterator HElement::attributeBegin()
    {
        return m_attribute.begin();
    }

    HElement::AttributeIterator HElement::attributeEnd()
    {
        return m_attribute.end();
    }

    std::string HElement::getAttribute(const std::string& k)
    {
        auto it = m_attribute.find(k);
        if (it != m_attribute.end()) {
            return it->second; // 返回值语义保持不变
        }
        return {};
    }

    HValue HElement::getElementById(const std::string& id)
    {
        for (HElement::ChildIterator it = m_children.begin(); it != m_children.end(); ++it) {
            if ((*it)->getAttribute("id") == id) return *it;

            HValue r = (*it)->getElementById(id);
            if (r) return r;
        }
        return {};
    }

    std::vector<HValue> HElement::getElementByClassName(const std::string& name)
    {
        std::vector<HValue> result;
        getElementByClassName(name, result);
        return result;
    }

    std::vector<HValue> HElement::getElementByTagName(const std::string& name)
    {
        std::vector<HValue> result;
        getElementByTagName(name, result);
        return result;
    }

    void HElement::selectElement(const std::string& rule, std::vector<HValue>& result)
    {
        if (rule.empty() || rule.at(0) != '/' || m_name == "plain") return;

        std::string::size_type pos = 0;
        if (rule.size() >= 2 && rule.at(1) == '/') {
            std::vector<HValue> temp;
            getAllElement(temp);
            pos = 1;
            std::string next = rule.substr(pos);
            if (next.empty()) {
                for (const auto & i : temp) {
                    insertIfNotExists(result, i);
                }
            }
            else {
                for (const auto & i : temp) {
                    i->selectElement(next, result);
                }
            }
            return;
        }

        // 解析一段 /xxx[@attr=’v’] 这样的规则
        std::string::size_type p = rule.find('/', 1);
        std::string line;
        if (p == std::string::npos) {
            line = rule;
            pos = rule.size();
        }
        else {
            line = rule.substr(0, p);
            pos = p;
        }

        enum { x_ele, x_wait_attr, x_attr, x_val };
        std::string ele, attr, oper, val, cond;
        int state = x_ele;

        for (p = 1; p < line.size(); ) {
            char c = line.at(p++);
            switch (state) {
                case x_ele:
                    if (c == '@') {
                        state = x_attr;
                    } else if (c == '!') {
                        state = x_wait_attr;
                        cond.push_back(c);
                    } else if (c == '[') {
                        state = x_wait_attr;
                    } else {
                        ele.push_back(c);
                    }
                    break;
                case x_wait_attr:
                    if (c == '@') state = x_attr;
                    else if (c == '!') cond.push_back(c);
                    break;
                case x_attr:
                    if (c == '!') {
                        oper.push_back(c);
                    } else if (c == '=') {
                        oper.push_back(c);
                        state = x_val;
                    } else if (c == ']') {
                        state = x_ele;
                    } else {
                        attr.push_back(c);
                    }
                    break;
                case x_val:
                    if (c == ']') {
                        state = x_ele;
                    } else {
                        val.push_back(c);
                    }
                    break;
            }
        }

        if (!val.empty() && (val.front() == '\'' || val.front() == '"')) {
            val.erase(val.begin());
        }
        if (!val.empty() && (val.back() == '\'' || val.back() == '"')) {
            val.pop_back();
        }

        bool matched = true;
        if (!ele.empty() && m_name != ele) {
            matched = false;
        }

        auto contains_class = [](const std::string& classes, const std::string& needle)->bool {
            auto set = HElement::splitClassName(classes);
            return set.find(needle) != set.end();
        };

        if (cond == "!") {
            if (!attr.empty() && matched) {
                auto it = m_attribute.find(attr);
                if (!oper.empty()) {
                    std::string v = (it != m_attribute.end()) ? it->second : std::string();
                    if (oper == "=") {
                        if (v == val) matched = false;
                        if (attr == "class" && contains_class(getAttribute("class"), val)) matched = false;
                    } else if (oper == "!=") {
                        if (v == val) matched = false;
                        if (attr == "class" && !contains_class(getAttribute("class"), val)) matched = false;
                    }
                } else {
                    if (it != m_attribute.end()) matched = false;
                }
            }
        } else {
            if (!attr.empty() && matched) {
                auto it = m_attribute.find(attr);
                if (it == m_attribute.end()) {
                    matched = false;
                } else {
                    const std::string& v = it->second;
                    if (oper == "=") {
                        if (v != val) matched = false;
                        if (attr == "class" && !contains_class(getAttribute("class"), val)) matched = false;
                    } else if (oper == "!=") {
                        if (v == val) matched = false;
                        if (attr == "class" && contains_class(getAttribute("class"), val)) matched = false;
                    }
                }
            }
        }

        std::string next = rule.substr(pos);
        if (matched) {
            if (next.empty()) {
                insertIfNotExists(result, shared_from_this());
            } else {
                for (auto it = childBegin(); it != childEnd(); ++it) {
                    (*it)->selectElement(next, result);
                }
            }
        }
    }

    HValue HElement::getParent()
    {
        return m_parent.lock();
    }

    const std::string& HElement::getValue()
    {
        if (m_value.empty() && m_children.size() == 1 && m_children[0]->getName() == "plain") {
            return m_children[0]->getValue();
        }
        return m_value;
    }

    const std::string& HElement::getName()
    {
        return m_name;
    }

    std::string HElement::toPlainText() const
    {
        std::string str;
        asPlainText(str);
        return str;
    }

    void HElement::asPlainText(std::string& str) const
    {
        if (m_name == "head" || m_name == "meta" || m_name == "style" || m_name == "script" || m_name == "link") {
            return;
        }

        if (m_name == "plain") {
            str.append(m_value);
            return;
        }

        for (size_t i = 0; i < m_children.size(); ) {
            m_children[i]->asPlainText(str);

            if (++i < m_children.size()) {
                const std::string& ele = m_children[i]->getName();
                if (ele == "td") {
                    str.append("\t");
                }
                else if (ele == "tr" || ele == "br" || ele == "div" || ele == "p" || ele == "hr" || ele == "area" ||
                         ele == "h1" || ele == "h2" || ele == "h3" || ele == "h4" || ele == "h5" || ele == "h6" || ele == "h7") {
                    str.append("\n");
                }
            }
        }
    }

    std::string HElement::toHtml() const
    {
        std::string str;
        asHtml(str);
        return str;
    }

    void HElement::asHtml(std::string& str) const
    {
        if (m_name.empty()) {
            for (const auto & i : m_children) {
                i->asHtml(str);
            }
            return;
        }
        else if (m_name == "plain") {
            str.append(m_value);
            return;
        }

        str.reserve(str.size() + m_name.size() + m_value.size() + 16 + m_attribute.size() * 8);

        str.append("<");
        str.append(m_name);

        for (const auto& kv : m_attribute) {
            str.push_back(' ');
            str.append(kv.first);
            str.append("=\"");
            str.append(kv.second);
            str.push_back('"');
        }
        str.push_back('>');

        if (m_children.empty()) {
            str.append(m_value);
        }
        else {
            for (const auto & i : m_children) {
                i->asHtml(str);
            }
        }

        str.append("</");
        str.append(m_name);
        str.append(">");
    }

    void HElement::getElementByClassName(const std::string& name, std::vector<HValue>& result)
    {
        std::set<std::string> class_name = splitClassName(name);

        for (HElement::ChildIterator it = m_children.begin(); it != m_children.end(); ++it) {
            std::set<std::string> attr_class = splitClassName((*it)->getAttribute("class"));

            bool allFound = true;
            for (const auto& cls : class_name) {
                if (attr_class.find(cls) == attr_class.end()) {
                    allFound = false;
                    break;
                }
            }

            if (allFound) {
                insertIfNotExists(result, *it);
            }

            (*it)->getElementByClassName(name, result);
        }
    }

    void HElement::getElementByTagName(const std::string& name, std::vector<HValue>& result)
    {
        for (HElement::ChildIterator it = m_children.begin(); it != m_children.end(); ++it) {
            if ((*it)->m_name == name)
                insertIfNotExists(result, *it);

            (*it)->getElementByTagName(name, result);
        }
    }

    void HElement::getAllElement(std::vector<HValue>& result) {
        for (const auto & i : m_children) {
            insertIfNotExists(result, i);
            i->getAllElement(result);
        }
    }

    void HElement::parse(const std::string& attr)
    {
        size_t i = 0, n = attr.size();

        auto skip_spaces = [&](size_t& j) {
            while (j < n && std::isspace(static_cast<unsigned char>(attr[j]))) ++j;
        };

        std::string key, val;
        while (i < n) {
            skip_spaces(i);
            if (i >= n) break;

            size_t key_begin = i;
            while (i < n) {
                char c = attr[i];
                if (c == '=' || std::isspace(static_cast<unsigned char>(c))) break;
                ++i;
            }
            key.assign(attr.data() + key_begin, i - key_begin);

            skip_spaces(i);

            val.clear();

            if (i >= n || attr[i] != '=') {
                if (!key.empty()) {
                    m_attribute[key] = val;
                }
                while (i < n && !std::isspace(static_cast<unsigned char>(attr[i]))) ++i;
                continue;
            }

            ++i;
            skip_spaces(i);
            if (i >= n) {
                m_attribute[key] = val;
                break;
            }

            char quote = 0;
            if (attr[i] == '"' || attr[i] == '\'') {
                quote = attr[i++];
                size_t vbeg = i;
                while (i < n && attr[i] != quote) ++i;
                val.assign(attr.data() + vbeg, i - vbeg);
                if (i < n && attr[i] == quote) ++i; // 跳过收尾引号
            } else {
                size_t vbeg = i;
                while (i < n && !std::isspace(static_cast<unsigned char>(attr[i]))) ++i;
                val.assign(attr.data() + vbeg, i - vbeg);
            }

            if (!key.empty()) {
                m_attribute[key] = val;
            }
        }

        if (!m_value.empty()) {
            size_t first = m_value.find_first_not_of(' ');
            if (first == std::string::npos) {
                m_value.clear();
            } else {
                if (first > 0) m_value.erase(0, first);
                size_t last = m_value.find_last_not_of(' ');
                if (last != std::string::npos && last + 1 < m_value.size()) {
                    m_value.erase(last + 1);
                }
            }
        }
    }

    std::set<std::string> HElement::splitClassName(const std::string& name)
    {
        std::set<std::string> class_names;
        size_t i = 0, n = name.size();
        while (i < n) {
            while (i < n && std::isspace(static_cast<unsigned char>(name[i]))) ++i;
            size_t start = i;
            while (i < n && !std::isspace(static_cast<unsigned char>(name[i]))) ++i;
            if (i > start) {
                class_names.insert(name.substr(start, i - start));
            }
        }
        return class_names;
    }

    void HElement::insertIfNotExists(std::vector<HValue>& vec, const HValue& ele)
    {
        for (const auto & i : vec) {
            if (i == ele) return;
        }
        vec.push_back(ele);
    }

}