#include <tokenizer.hpp>

SCDDTokenizer::SCDDTokenizer(std::string const &src) : m_source(std::move(src)) {}

std::vector<Token> SCDDTokenizer::tokenize() {
    std::vector<Token> tokens;
    std::string buf;
    while (peek().has_value()) {
        if (std::isalpha(peek().value())) {
            buf.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value())) {
                buf.push_back(consume());
            }
            if (buf == "exit") {
                tokens.push_back({ .type = TokenType::_exit });
                buf.clear();
                continue;
            } else if (buf == "let") {
                tokens.push_back({ .type = TokenType::let });
                buf.clear();
                continue;
            } else {
                tokens.push_back({ .type = TokenType::ident, .value = buf });
                buf.clear();
                continue;
            }
        }
        else if (std::isdigit(peek().value())) {
            buf.push_back(consume());
            while (peek().has_value() && std::isdigit(peek().value())) {
                buf.push_back(consume()); 
            }
            tokens.push_back({ .type = TokenType::int_lit, .value = buf });
            buf.clear();
            continue;
        }
        else if (peek().value() == '(') {
            consume();
            tokens.push_back({ .type = TokenType::o_paren });
            continue;
        }
        else if (peek().value() == ')') {
            consume();
            tokens.push_back({ .type = TokenType::c_paren });
            continue;
        }
        else if (peek().value() == ';') {
            consume();
            tokens.push_back({ .type = TokenType::semi });
            continue;
        }
        else if (peek().value() == '=') {
            consume();
            tokens.push_back({ .type = TokenType::eq });
            continue;
        }
        else if (peek().value() == '+') {
            consume();
            tokens.push_back({ .type = TokenType::add });
            continue;
        }
        else if (std::isspace(peek().value())) {
            consume();
            continue;
        }
        else {
            fatal_error("unknown syntax"); 
        }
    }
    m_index = 0;
    return tokens;
}

std::optional<char> SCDDTokenizer::peek(s32 offset) const {
    if (m_index + offset >= m_source.length()) {
        return {};
    }
    else {
        return m_source.at(m_index + offset);
    }
}

char SCDDTokenizer::consume() {
    return m_source.at(m_index++);
}