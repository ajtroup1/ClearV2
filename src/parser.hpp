// Contians all functionality relevant to parsing tokens into an AST
#pragma once

#include <vector>
#include <optional>
#include <variant>

#include "tokenization.hpp"
#include "arena.hpp"

// nodes used to construct the AST
// ranging from furthest from the root (top of the list) to the root (bottom of the list)

// contained in expressions
struct NodeExprIntLit {
    Token int_lit;
};
struct NodeExprIdent {
    Token ident;
};

struct NodeExpr;

struct NodeBinExprAdd {
    NodeExpr* lhs;
    NodeExpr* rhs;
    
};
// struct NodeBinExprMulti {
//     NodeExpr* lhs;
//     NodeExpr* rhs;
// };

struct NodeTermIntLit {
    Token int_lit;
};

struct NodeTermIdent {
    Token ident;
};

struct NodeTerm {
    std::variant<NodeTermIntLit*, NodeTermIdent*> var;
};

// struct NodeBinExpr {
//     std::variant<NodeBinExprAdd*, NodeBinExprMulti*> var;
// };
struct NodeBinExpr {
    NodeBinExprAdd* add;
};

struct NodeExpr {
    std::variant<NodeTerm*, NodeBinExpr*> var;
};

// contained in statements
struct NodeStmtExit {
    NodeExpr* expr;
};
struct NodeStmtLet {
    Token ident;
    NodeExpr* expr;
};

struct NodeStmt {
    std::variant<NodeStmtExit*, NodeStmtLet*> var;
};


// root node (program)
struct NodeProgram {
    std::vector<NodeStmt*> stmts;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens)),
        m_allocator(1024 * 1024 * 4) // 4mb
    {
    }

    std::optional<NodeTerm*> parse_term() {
        // handle integer literal
        if (auto int_lit = try_consume(TokenType::int_lit)) {
            auto term_int_lit = m_allocator.alloc<NodeTermIntLit>();
            term_int_lit->int_lit = int_lit.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = term_int_lit;
            return term;
        }
        // handle identifier
        else if (auto ident = try_consume(TokenType::ident)) {
            auto term_ident = m_allocator.alloc<NodeTermIdent>();
            term_ident->ident = ident.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = term_ident;
            return term;
        } 
        else {
            return {};
        }
    }

    // parses an expression, which can be a(n):
    // integer literal, identifier
    std::optional<NodeExpr*> parse_expr() {
        if (auto term = parse_term()) {
            if (try_consume(TokenType::plus).has_value()) {
                auto bin_expr = m_allocator.alloc<NodeBinExpr>();
                auto bin_expr_add = m_allocator.alloc<NodeBinExprAdd>();
                auto lhs_expr = m_allocator.alloc<NodeExpr>();
                lhs_expr->var = term.value();
                bin_expr_add->lhs = lhs_expr;
                if (auto rhs = parse_expr()) {
                    bin_expr_add->rhs = rhs.value();
                    // bin_expr->var = bin_expr_add;
                    bin_expr->add = bin_expr_add; // MADE TEMP CHANGE HERE
                    auto expr = m_allocator.alloc<NodeExpr>();
                    expr->var = bin_expr;
                    return expr;
                } else {
                    std::cerr << "Unable to parse expression on right hand side" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                auto expr = m_allocator.alloc<NodeExpr>();
                expr->var = term.value();
                return expr;
            }
        } else {
            return {};
        }
        // handles binary expressions
        // if (auto bin_expr = parse_bin_expr()) {
        //     auto expr = m_allocator.alloc<NodeExpr>();
        //     expr->var = bin_expr.value();
        //     return expr;
        // }
        // unhandled expression
    }

    // parses an idividual statement from the vector of statements contained in program
    // available statements include:
    // exit, let
    std::optional<NodeStmt*> parse_stmt() {
        // handle exit stmt
        if (peek().value().type == TokenType::exit && peek(1).has_value() && peek(1).value().type == TokenType::open_paren) {
            consume(); 
            consume(); 
            auto stmt_exit = m_allocator.alloc<NodeStmtExit>();
            if (auto node_expr = parse_expr()) { 
                stmt_exit->expr = node_expr.value();
            } else {
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            try_consume(TokenType::close_paren, "Expected `)`");
            try_consume(TokenType::semi, "Expected `;`");

            auto stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = stmt_exit; 
            return stmt;
        }
        // handle let stmt
        else if (peek().has_value() && peek().value().type == TokenType::let && peek(1).has_value() && peek(1).value().type == TokenType::ident && peek(2).has_value() && peek(2).value().type == TokenType::eq) {
            consume(); // let

            // allocate and store ident
            auto stmt_let = m_allocator.alloc<NodeStmtLet>();
            stmt_let->ident = consume();

            consume(); // '='

            if (auto expr = parse_expr()) {
                stmt_let->expr = expr.value();
            } else {
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            try_consume(TokenType::semi, "Expected `;`");

            // return NodeStmt {.var = stmt_let};
            auto stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = stmt_let;
            return stmt;
        }
        // unhandled stmt
        else {
            return {};
        }
    }

    // parses the program node's statements in a loop
    // top level parsing function
    std::optional<NodeProgram> parse_prog() {
        NodeProgram prog;
        // as long as there is a statement ahead in the tokens, parse it
        while (peek().has_value()) {
            if (auto stmt = parse_stmt()) {
                prog.stmts.push_back(stmt.value()); // parse individual stmt
            } else {
                std::cerr << "failed to parse statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // return the root node of the program
        return prog;
    }

private:
    // peek ahead, this time with tokens, not just chars
    [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const {
        if (m_index + offset >= m_tokens.size()) {
            return {};
        }

        return m_tokens.at(m_index + offset);
    }

    inline Token try_consume(TokenType type, const std::string& err_msg) {
        if (peek().has_value() && peek().value().type == type) {
            return consume();
        } else {
            std::cerr << err_msg << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    inline std::optional<Token> try_consume(TokenType type) {
        if (peek().has_value() && peek().value().type == type) {
            return consume();
        } else {
            return {};
        }
    }

    // advance to next token
    inline Token consume() {
        return m_tokens.at(m_index++);
    }

    // memeber vars
    const std::vector<Token> m_tokens; // stream of all tokens (sequential)
    size_t m_index = 0; // pointer for current position in tokens vector
    ArenaAllocator m_allocator;
};
