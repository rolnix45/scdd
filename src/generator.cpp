#include <generator.hpp>

#include <cassert>

SCDDGenerator::SCDDGenerator(node::prog prog) : m_prog(std::move(prog)) {}

void SCDDGenerator::gen_term(const node::term* term) {
    struct term_visitor {
        SCDDGenerator* gen;
        void operator()(const node::term_int_lit* term_int_lit) const {
            gen->m_output << "  mov rax, " << term_int_lit->int_lit.value.value() << '\n';
            gen->push("rax");
        }
        void operator()(const node::term_ident* term_ident) const {
            if (!gen->m_vars.contains(term_ident->ident.value.value())) {
                fatal_error("undeclared identifier");
            }
            auto const &var = gen->m_vars.at(term_ident->ident.value.value());
            std::stringstream offset;
            offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_location - 1) * 8 << "]";
            gen->push(offset.str());
        }
    };
    term_visitor visitor { .gen = this };
    std::visit(visitor, term->var);
}

void SCDDGenerator::gen_expr(const node::expr* expression) {
    struct expr_visitor {
        SCDDGenerator* gen;
        void operator()(const node::term* term) const {
            gen->gen_term(term);
        }
        void operator()(const node::bin_expr* bin_expr) const {
            gen->gen_expr(bin_expr->add->lhs);
            gen->gen_expr(bin_expr->add->rhs);
            gen->pop("rax");
            gen->pop("rbx");
            gen->m_output << "  add rax, rbx\n";
            gen->push("rax");
        }
    };
    expr_visitor visitor { .gen = this };
    std::visit(visitor, expression->var);
}

void SCDDGenerator::gen_stmt(const node::stmt* statement) {
    struct stmt_visitor {
        SCDDGenerator* gen;
        void operator()(const node::stmt_exit* stmt_exit) const {
            gen->gen_expr(stmt_exit->expr);
            gen->m_output << "  mov rax, 60" << '\n';
            gen->pop("rdi");
            gen->m_output << "  syscall\n";
        }
        void operator()(const node::stmt_let* stmt_let) const {
            if (gen->m_vars.contains(stmt_let->ident.value.value())) {
                fatal_error("identifier already used");
            }
            gen->m_vars.insert({ stmt_let->ident.value.value(), var { .stack_location = gen->m_stack_size }});
            gen->gen_expr(stmt_let->expr);
        }
    };
    stmt_visitor visitor { .gen = this };
    std::visit(visitor, statement->var);
}

std::string SCDDGenerator::gen_prog() {
    m_output << "global _start\n_start:\n";
    for (const node::stmt* stmt : m_prog.statements) {
        gen_stmt(stmt);
    }
    return m_output.str();
}

void SCDDGenerator::push(std::string const &reg) {
    m_output << "  push " << reg << '\n';
    m_stack_size++;
}

void SCDDGenerator::pop(std::string const &reg) {
    m_output << "  pop " << reg << '\n';
    m_stack_size--;
}
