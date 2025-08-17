// Created by cubevlmu on 2025/8/17.
// Copyright (c) 2025 Flybird Games. All rights reserved.

// This project contains code from [https://github.com/rangerlee/htmlparser], licensed under Apache License 2.0.
// Modifications by cubevlmu are licensed under MIT License.

#include "HParser.hpp"

#include <memory>

#include "HElement.hpp"
#include "HDocument.hpp"
#include "Logger.hpp"

namespace html {
    static const std::string s_token[] = { "br", "hr", "img", "input", "link", "meta",
                                         "area", "base", "col", "command", "embed", "keygen", "param", "source", "track", "wbr" };

    HParser::HParser() {
        m_self_closing_tags.insert(s_token, s_token + sizeof(s_token) / sizeof(s_token[0]));
    }
    
    HDoc HParser::parse(const char* data, psize len)
    {
        m_stream = data;
        m_length = len;
        psize index = 0;
        m_root = std::make_shared<HElement>();
        while (m_length > index) {
            char input = m_stream[index];
            if (input == '\r' || input == '\n' || input == '\t' || input == ' ') {
                index++;
            }
            else if (input == '<') {
                index = parseElement(index, m_root);
            }
            else {
                break;
            }
        }

        return std::make_shared<HDocument>(m_root);
    }

    psize HParser::parseElement(psize index, HValue& parent) {
        if (index >= m_length || m_stream[index] != '<')
            return index;

        char next = (index + 1 < m_length) ? m_stream[index + 1] : 0;
        if (next == '!') {
            if (strncmp(m_stream + index, "<!--", 4) == 0) {
                return skipUntil(index + 4, "-->");
            }
            return skipUntil(index + 2, '>');
        } else if (next == '/') {
            return skipUntil(index, '>');
        } else if (next == '?') {
            // XML PI
            return skipUntil(index + 2, "?>");
        }

        HValue self(new HElement(parent));

        enum State {
            TAG_NAME,
            ATTRS,
            CONTENT,
            TAG_END
        };

        State state = TAG_NAME;
        index++; // skip '<'
        std::string attr;
        psize start = 0;

        while (index < m_length) {
            switch (state) {
                case TAG_NAME: {
                    start = index;
                    while (index < m_length && !isspace(m_stream[index]) &&
                           m_stream[index] != '>' && m_stream[index] != '/')
                    {
                        index++;
                    }
                    self->m_name.assign(m_stream + start, index - start);

                    while (index < m_length && isspace(m_stream[index])) index++;

                    if (index < m_length && m_stream[index] == '>') {
                        if (m_self_closing_tags.count(self->m_name)) {
                            self->parse(attr);
                            parent->m_children.push_back(self);
                            return ++index;
                        }
                        state = CONTENT;
                        index++;
                    } else if (index < m_length && m_stream[index] == '/') {
                        // 自闭合
                        index = skipUntil(index, '>');
                        self->parse(attr);
                        parent->m_children.push_back(self);
                        return index;
                    } else {
                        state = ATTRS;
                    }
                } break;

                case ATTRS: {
                    start = index;
                    while (index < m_length && m_stream[index] != '>' && m_stream[index] != '/') {
                        index++;
                    }
                    attr.assign(m_stream + start, index - start);

                    if (index < m_length && m_stream[index] == '>') {
                        if (m_self_closing_tags.count(self->m_name)) {
                            self->parse(attr);
                            parent->m_children.push_back(self);
                            return ++index;
                        }
                        self->parse(attr);
                        state = CONTENT;
                        index++;
                    } else if (index < m_length && m_stream[index] == '/') {
                        index = skipUntil(index, '>');
                        self->parse(attr);
                        parent->m_children.push_back(self);
                        return index;
                    }
                } break;

                case CONTENT: {
                    if (self->m_name == "script" || self->m_name == "noscript" || self->m_name == "style") {
                        std::string close = "</" + self->m_name + ">";
                        psize pre = index;
                        index = skipUntil(index, close.c_str());
                        if (index > (pre + close.size()))
                            self->m_value.assign(m_stream + pre, index - pre - close.size());
                        parent->m_children.push_back(self);
                        return index;
                    }

                    if (m_stream[index] == '<') {
                        if (!self->m_value.empty()) {
                            HValue textNode(new HElement(self));
                            textNode->m_name = "plain";
                            textNode->m_value.swap(self->m_value);
                            self->m_children.push_back(textNode);
                        }

                        if (index + 1 < m_length && m_stream[index + 1] == '/') {
                            state = TAG_END;
                        } else {
                            index = parseElement(index, self);
                        }
                    } else {
                        start = index;
                        while (index < m_length && m_stream[index] != '<') {
                            index++;
                        }
                        self->m_value.append(m_stream + start, index - start);
                    }
                } break;

                case TAG_END: {
                    index += 2; // skip "</"
                    start = index;
                    while (index < m_length && m_stream[index] != '>') index++;
                    std::string closeName(m_stream + start, index - start);

                    if (closeName == self->m_name) {
                        self->parse(attr);
                        parent->m_children.push_back(self);
                        return ++index;
                    } else {
                        LogError("unexpected closed element </{}> for <{}>",
                                 closeName, self->m_name);
                        return index < m_length ? ++index : index;
                    }
                } break;
            }
        }

        return index;
    }

    psize HParser::skipUntil(psize index, const char* token) {
        const char* found = std::strstr(m_stream + index, token);
        if (found) return (found - m_stream) + strlen(token);
        return m_length;
    }

    psize HParser::skipUntil(psize index, const char data)
    {
        while (m_length > index) {
            if (m_stream[index] == data) {
                return ++index;
            }
            else {
                index++;
            }
        }

        return index;
    }
}