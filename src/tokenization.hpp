// Contains functionality necessary to convert source code to a stream of tokens
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>

// enum class is some C++ wizardry that allows for comparison of integers using string 'identifiers'
enum class TokenType {
    exit,
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    eq,
    plus,
    multi
};

// Actual token class to be referenced throughout the parser
// Contains both the type of token and optionally an associated literal value
struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src)
        : m_src(std::move(src))
    {
    }

    // function to tokenize entire content string
    // returns a vector of tokens, containing every token in the program    
    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf; // buffer for containing a sequence of chars to be analyzed and used for tokenization at a later time
        
        // read through the src code char by char until peek no longer returns a value
        while (peek().has_value()) {
            // if we encounter an alphabetic char, it must be either a keyword or identifier
            if (std::isalpha(peek().value())) {
                buf.push_back(consume()); // consume initial alpha char

                // identifiers or keywords may continue with alpha or num chars
                while(peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(consume()); // continue consuming
                }

                // now, we have the word and need to check if it's a keyword. otherwise, it must be an identifier
                // std::cout << buf << std::endl;

                // keyword checks
                if (buf == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                } else if (buf == "let") {
                    tokens.push_back({.type = TokenType::let});
                    buf.clear();
                    continue;
                }
                
                // buffer is an identifier
                else {
                    tokens.push_back({.type = TokenType::ident, .value = buf});
                    buf.clear();
                    continue;
                }
            } 

            // if we encounter a digit, we must be reading an integer literal
            else if (std::isdigit(peek().value())) {
                buf.push_back(consume()); // consume the initial int

                // integers may continue as digits
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buf.push_back(consume()); // continue reading into the buffer
                }

                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear();
                continue;
            } 
            // misc symbols / chars checking
            else if (peek().value() == '(') {
                consume();
                tokens.push_back({.type = TokenType::open_paren});
                continue;
            }
            else if (peek().value() == ')') {
                consume();
                tokens.push_back({.type = TokenType::close_paren});
                continue;
            }
            else if (peek().value() == ';') {
                tokens.push_back({.type = TokenType::semi});
                consume();
                continue;
            } 
            else if (peek().value() == '=') {
                tokens.push_back({.type = TokenType::eq});
                consume();
                continue;
            }
            else if (peek().value() == '+') {
                tokens.push_back({.type = TokenType::plus});
                consume();
                continue;
            }
            else if (peek().value() == '*') {
                tokens.push_back({.type = TokenType::multi});
                consume();
                continue;
            }
            else if (std::isspace(peek().value())) {
                consume();
                continue;
            } 
            
            // unidentified char
            else {
                std::cerr << "Illegal character: " << peek().value() << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        m_index = 0;
        return tokens;
    }
private:
    // peek looks ahead in the src code, usually to the next char, but optionally more
    [[nodiscard]] inline std::optional<char> peek(int offset = 0) const {
        if (m_index + offset >= m_src.length()) {
            return {};
        }

        return m_src.at(m_index + offset);
    }

    // actually advances to the next char in the src
    inline char consume() {
        return m_src.at(m_index++);
    }

    // member vars
    const std::string m_src; // entire src code
    size_t m_index = 0; // position pointer
};