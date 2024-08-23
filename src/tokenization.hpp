#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>

// enum class is some C++ wizardry that allows for comparison of integers using string 'identifiers'
enum class TokenType {
    exit,
    int_lit,
    semi
};

// Actual token class to be referenced throughout the parser
// Contains both the type of token and optionally an associated literal value
struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    inline Tokenizer(std::string src)
        : m_src(std::move(src))
    {
    }

    // function to tokenize entire content string
    // returns a vector of tokens, containing every token in the program    
    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf; // buffer for containing a sequence of chars to be analyzed and used for tokenization at a later time

        // read through the source code char by char
        for (int i = 0; i < str.length(); i++) {
            char c = str.at(i);

            // if the character is alphabetic, it must be a keyword or identifier
            // this cannot start with a number, but can continue with a number
            if (std::isalpha(c)) {
                // consume first char
                buf.push_back(c);
                i++;
                // as long as it keeps reading in a letter or number, keep reading into the buffer
                while (std::isalnum(str.at(i))) {
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;

                // check if buffer is a keyword
                if (buf == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                } 
                
                // if the buffer isn't a keyword, it must be an identifier
                else {
                    // for now, just return err if not keyword
                    std::cerr << "Expected keyword..." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            // if the character is a digit, it must be a number (int, float...)
            // at this point, only ints
            else if (std::isdigit(c)) {
                // same logic for reading in keywords / identifiers
                buf.push_back(c);
                i++;
                while (std::isdigit(str.at(i))) {
                    buf.push_back(str.at(i));
                    i++;

                }
                i--;
                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear();
            }

            // basic misc char recognition. pretty self explainatory
            else if (c == ';') {
                tokens.push_back({.type = TokenType::semi});
            }

            // clear doesn't care about whitespace, so it just skips if it encounters any whitespace chars
            else if (std::isspace(c)) {
                continue;
            }

            // char wasn't anything i accounted for, so i guess it's illegal
            else {
                std::cerr << "Invalid character" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // return the list of all tokens extracted from the source code
        return tokens;
    }
private:
    const std::string m_src;
};