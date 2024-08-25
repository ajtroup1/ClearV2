#pragma once

#include <sstream>
#include <unordered_map>

#include "parser.hpp"
#include <cassert>

class Generator {
public:
    inline explicit Generator(NodeProgram root) 
        : m_prog(std::move(root))
    {
    }

    void gen_term(const NodeTerm* term) {
        struct TermVisistor {
            Generator* gen;
            
        };
    }

    // **NOTE
    // a 'visitor' is a design pattern that maps input to functionality based on the input's type
    // example: if an expr with an int lit node gets passed in, the associated int lit functionality gets ran

    // handle generation of assembly for expressions
    void gen_expr(const NodeExpr* expr) {
        // declare the visitor for expressions
        struct ExprVisitor {
            Generator* gen;

            // handle integer literal expression
            void operator()(const NodeExprIntLit* expr_int_lit) const {
                gen->m_output << "    mov rax, " << expr_int_lit->int_lit.value.value() << "\n";
                gen->push("rax");
            }
            // handle identifier expressions
            void operator()(const NodeExprIdent* expr_ident) const {
                const auto& ident_value = expr_ident->ident.value.value();
                
                if (!gen->m_vars.contains(ident_value)) {
                    std::cerr << "Variable '" << ident_value << "' not declared" << std::endl;
                    exit(EXIT_FAILURE);
                }

                const auto& var = gen->m_vars.at(ident_value);

                std::stringstream offset;
                offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8 << "]";
                gen->push(offset.str());
            }
            // handles binary expressions
            void operator()(const NodeBinExpr* bin_expr) const {
                assert(false);
            }
        };

        ExprVisitor visitor({.gen = this});
        std::visit(visitor, expr->var);
    }

    // handle generation of assembly for statements
    void gen_stmt(const NodeStmt* stmt) {
        // define visitor for stmts
        struct StmtVisitor {
            Generator* gen;
            // handle exit stmt
            void operator()(const NodeStmtExit* stmt_exit) const {
                gen->gen_expr(stmt_exit->expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            //handle let stmt
            void operator()(const NodeStmtLet* stmt_let) {
                if (gen->m_vars.contains(stmt_let->ident.value.value())) {
                    std::cerr << "Identifier already used: " << stmt_let->ident.value.value() << std::endl;
                    exit(EXIT_FAILURE);
                }

                gen->m_vars.insert({stmt_let->ident.value.value(), Var {.stack_loc = gen->m_stack_size}});
                gen->gen_expr(stmt_let->expr);
            }
        };

        StmtVisitor visitor({.gen = this});
        std::visit(visitor, stmt->var);
    }

    // generates assembly for the entire program (root)
    [[nodiscard]] std::string generate_prog() {
        m_output << "global _start\n_start:\n"; // starter assembly. independant of other circumstances

        // generate the assembly for each stmt in the program
        for (const NodeStmt* stmt : m_prog.stmts) {
            gen_stmt(stmt);
        }

        // default exit if nothing is specified
        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall\n";

        // return entire assembly file as a string
        return m_output.str();
    }
    
private:
    // pushes a value from a given register and incriments stack size
    void push(const std::string& reg) {
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }
    // pops a value from a given register and decrements stack size
    void pop(const std::string& reg) {
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    // variable struct, contains information about a variable stored in the stack
    struct Var {
        size_t stack_loc;
    };

    // member vars
    const NodeProgram m_prog;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, Var> m_vars {};
};