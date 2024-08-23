// Contians all functionality relevant to parsing tokens into an AST
#pragma once

#include <vector>
#include <optional>

#include "tokenization.hpp"

// nodes used to construct the AST

struct NodeExpr {
    Token int_lit;
};
struct NodeExit {
    NodeExpr expr;

};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens)) 
        {
        }

    std::optional<NodeExit> parse() {
        std::optional<NodeExit> exit_node;
        while (peek().has_value()) {
            if (peek().value().type == TokenType::exit) {
                consume(); // consume exit token
                // 'auto' creates the node_expr in the if stmt
                // auto implicitly types node_expr as the return type of the expression, parse_expr()
                // since we're using std::optional, if parse_expr() returns a value (non-null), the if block is executed. If it returns null, the else block is executed.
                if (auto node_expr = parse_expr()) { 
                    exit_node = NodeExit{.expr = node_expr.value()};
                } else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Missing semicolon after exit" << std::endl;
                    exit(EXIT_FAILURE);
                }

                
            }
        }

        m_index = 0;
        return exit_node;
    }

    std::optional<NodeExpr> parse_expr() {
        if (peek().has_value() && peek().value().type == TokenType::int_lit) {
            return NodeExpr{.int_lit = consume()};
        } else {
            return {};
        }
    }
private:
    // peek ahead, this time with tokens, not just chars
    [[nodiscard]] inline std::optional<Token> peek(int ahead = 1) const {
        if (m_index + ahead > m_tokens.size()) {
            return {};
        }

        return m_tokens.at(m_index);
    }

    // advance to next token
    inline Token consume() {
        return m_tokens.at(m_index++);
    }

    // memeber vars
    const std::vector<Token> m_tokens; // stream of all tokens (sequential)
    size_t m_index = 0; // pointer for current position in tokens vector
};
