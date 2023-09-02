#include <parser.hpp>

SCDDParser::SCDDParser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_alloc(1024 * 1024 * 4) {}

std::optional<node::term*> SCDDParser::parse_term() {
    if (auto int_lit = try_consume(TokenType::int_lit)) {
        auto term_int_lit = m_alloc.alloc<node::term_int_lit>();
        term_int_lit->int_lit = int_lit.value();
        auto term = m_alloc.alloc<node::term>();
        term->var = term_int_lit;
        return term; 
    }
    else if (auto ident = try_consume(TokenType::ident)) {
        auto term_ident = m_alloc.alloc<node::term_ident>();
        term_ident->ident = ident.value();
        auto term = m_alloc.alloc<node::term>();
        term->var = term_ident;
        return term;
    }
    else return {};
}

std::optional<node::expr*> SCDDParser::parse_expression() {
    if (auto term = parse_term()) {
        if (try_consume(TokenType::add).has_value()) {
            auto bin_expr = m_alloc.alloc<node::bin_expr>();
            auto bin_expr_add = m_alloc.alloc<node::bin_expr_add>();
            auto lhs_expr = m_alloc.alloc<node::expr>();
            lhs_expr->var = term.value();
            bin_expr_add->lhs = lhs_expr;
            if (auto rhs = parse_expression()) {
                bin_expr_add->rhs = rhs.value();
                bin_expr->add = bin_expr_add;
                auto expr = m_alloc.alloc<node::expr>();
                expr->var = bin_expr;
                return expr;
            }
            else {
                fatal_error("expected expression");
            }
        }
        else {
            auto expr = m_alloc.alloc<node::expr>();
            expr->var = term.value();
            return expr;
        }
    }
    else return {};
}

std::optional<node::stmt*> SCDDParser::parse_statement() {
    if (peek().value().type == TokenType::_exit && peek(1).has_value() && peek(1).value().type == TokenType::o_paren) {
        consume();
        consume();
        auto stmt_exit = m_alloc.alloc<node::stmt_exit>();
        if (auto node_expr = parse_expression()) {
            stmt_exit->expr = node_expr.value();
        } 
        else {
            fatal_error("Invalid expression");
        }
        try_consume(TokenType::c_paren, "PARENTHESIS!!!!");
        try_consume(TokenType::semi, "SEMICOLON!!!!");
        auto stmt = m_alloc.alloc<node::stmt>();
        stmt->var = stmt_exit;
        return stmt;
    }
    else if (peek().value().type == TokenType::let && peek(1).has_value() && peek(1).value().type == TokenType::ident && peek(2).has_value() && peek(2).value().type == TokenType::eq) {
        consume();
        auto stmt_let = m_alloc.alloc<node::stmt_let>();
        stmt_let->ident = consume();
        consume();
        if (auto expr = parse_expression()) {
            stmt_let->expr = expr.value();
        }
        else {
            fatal_error("expression bad");
        }
        try_consume(TokenType::semi, "SEMICOLON!!!!");
        auto stmt = m_alloc.alloc<node::stmt>();
        stmt->var = stmt_let;
        return stmt;
    }
    else {
        return {};
    }
}

std::optional<node::prog> SCDDParser::parse_program() {
    node::prog prog;
    while (peek().has_value()) {
        if (auto stmt = parse_statement()) {
            prog.statements.push_back(stmt.value());
        }
        else {
            fatal_error("invalid statement");
        }
    }
    return prog;
}

std::optional<Token> SCDDParser::peek(s32 offset) const {
    if (m_index + offset >= m_tokens.size()) {
        return {};
    }
    else {
        return m_tokens.at(m_index + offset);
    }
}

Token SCDDParser::consume() {
    return m_tokens.at(m_index++);
}

std::optional<Token> SCDDParser::try_consume(TokenType type) {
    if (peek().has_value() && peek().value().type == type) {
        return consume();
    }
    else {
        return {};
    }
}

Token SCDDParser::try_consume(TokenType type, const std::string &error_msg) {
    if (peek().has_value() && peek().value().type == type) {
        return consume();
    }
    else {
        fatal_error(error_msg.c_str());
    }
}