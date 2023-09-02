#pragma once

#include <prerequisites.hpp>
#include <parser.hpp>

#include <string>
#include <sstream>
#include <unordered_map>

struct var {
    size_t stack_location;
};

class SCDDGenerator {
public:
    explicit SCDDGenerator(node::prog prog);
    void gen_term(const node::term* term);
    void gen_expr(const node::expr* expression);
    void gen_stmt(const node::stmt* statement);
    std::string gen_prog();
private:
    void push(std::string const &reg);
    void pop(std::string const &reg);
    node::prog const m_prog;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, var> m_vars;
};