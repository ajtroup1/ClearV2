// Contians all functionality relevant to parsing tokens into an AST
#pragma once

#include <vector>
#include <optional>
#include <variant>

#include "tokenization.hpp"

// nodes used to construct the AST

struct NodeExprIntLit {
    Token int_lit;
};

struct NodeExprIdent {
    Token ident;
};

struct NodeExpr {
    std::variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStmtExit {
    NodeExpr expr;
};
struct NodeStmtLet {
    Token ident;
    NodeExpr expr;
};

struct NodeStmt {
    std::variant<NodeStmtExit, NodeStmtLet> var;
};

struct NodeProgram {
    std::vector<NodeStmt> stmts;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens)) 
    {
    }

    std::optional<NodeExpr> parse_expr() {
        if (peek().has_value() && peek().value().type == TokenType::int_lit) {
            return NodeExpr{ .var = NodeExprIntLit{.int_lit = consume()}};
        } if (peek().has_value() && peek().value().type == TokenType::ident) {
            return NodeExpr{ .var = NodeExprIdent{.ident = consume()}};
        } else {
            return {};
        }
    }

    std::optional<NodeStmt> parse_stmt() {
        if (peek().value().type == TokenType::exit && peek(1).has_value() && peek(1).value().type == TokenType::open_paren) {
                consume(); 
                consume(); 
                NodeStmtExit stmt_exit;
                if (auto node_expr = parse_expr()) { 
                    stmt_exit = {.expr = node_expr.value()};
                } else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if (peek().has_value() && peek().value().type == TokenType::close_paren) {
                    consume();
                } else {
                    std::cerr << "Missing ')' after exit" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peek().has_value() && peek().value().type == TokenType::semi) {
                    consume();
                } else {
                    std::cerr << "Missing ';' after exit" << std::endl;
                    exit(EXIT_FAILURE);
                }

                return NodeStmt{.var = stmt_exit};
            }
        else if (peek().has_value() && peek().value().type == TokenType::let && peek(1).has_value() && peek(1).value().type == TokenType::ident && peek(2).has_value() && peek(2).value().type == TokenType::eq) {
            consume(); // let
            auto stmt_let = NodeStmtLet {.ident = consume()};
            consume(); // '='
            if (auto expr = parse_expr()) {
                stmt_let.expr = expr.value();
            } else {
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peek().has_value() && peek().value().type == TokenType::semi) {
                consume(); // ';'
            } else {
                std::cerr << "Expected ';'" << std::endl;
                exit(EXIT_FAILURE);
            }

            return NodeStmt {.var = stmt_let};
        }
        else {
            return {};
        }
    }

    std::optional<NodeProgram> parse_prog() {
        NodeProgram prog;
        while (peek().has_value()) {
            if (auto stmt = parse_stmt()) {
                prog.stmts.push_back(stmt.value());
            } else {
                std::cerr << "failed to parse statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

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

    // advance to next token
    inline Token consume() {
        return m_tokens.at(m_index++);
    }

    // memeber vars
    const std::vector<Token> m_tokens; // stream of all tokens (sequential)
    size_t m_index = 0; // pointer for current position in tokens vector
};
