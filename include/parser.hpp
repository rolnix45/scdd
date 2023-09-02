#pragma once

#include <prerequisites.hpp>
#include <arena.hpp>

#include <vector>
#include <optional>
#include <variant>

namespace node {
    struct term_int_lit {
        Token int_lit;
    };

    struct term_ident {
        Token ident;
    };

    struct expr;

    struct bin_expr_add {
        node::expr* lhs;
        node::expr* rhs;
    };
    
    // TODO: multiplication

    //struct bin_expr_mult {
    //    node::expr* lhs;
    //    node::expr* rhs;
    //};
    
    struct bin_expr {
        //std::variant<node::bin_expr_add*, node::bin_expr_mult*> var;    
        node::bin_expr_add* add;    
    };
    
    struct term {
        std::variant<node::term_int_lit*, node::term_ident*> var;
    };
    
    struct expr {
        std::variant<node::term*, node::bin_expr*> var;
    };

    struct stmt_exit {
        node::expr* expr;
    };

    struct stmt_let {
        Token ident;
        node::expr* expr;
    };

    struct stmt {
        std::variant<node::stmt_exit*, node::stmt_let*> var;
    };

    struct prog {
        std::vector<node::stmt*> statements;
    };
}

class SCDDParser {
public:
    explicit SCDDParser(std::vector<Token> tokens);
    std::optional<node::bin_expr*> parse_bin_expression();
    std::optional<node::term*> parse_term();
    std::optional<node::expr*> parse_expression();
    std::optional<node::stmt*> parse_statement();
    std::optional<node::prog> parse_program();
private:
    size_t m_index = 0;
    std::vector<Token> const m_tokens;
    [[nodiscard]] std::optional<Token> peek(s32 offset = 0) const;
    Token consume();
    std::optional<Token> try_consume(TokenType type);
    Token try_consume(TokenType type, const std::string &error_msg);
    ArenaAllocator m_alloc;
};