#pragma once

#include <prerequisites.hpp>

#include <string>
#include <vector>
#include <optional>

class SCDDTokenizer {
public:
    explicit SCDDTokenizer(std::string const &src);
    std::vector<Token> tokenize();
private:
    size_t m_index = 0;

    std::string const m_source;
    [[nodiscard]] std::optional<char> peek(s32 offset = 0) const;
    char consume();
};