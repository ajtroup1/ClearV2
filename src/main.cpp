// TODO: 8:21 pt.2

#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "parser.hpp"
#include "tokenization.hpp"
#include "generation.hpp"

// temporary function to convert tokens to assambly
std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";

    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);

        if (token.type == TokenType::exit) {
            if (i + 1 < tokens.size() && tokens.at(i+1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i+2).type == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i+1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }

    return output.str();
}

int main(int argc, char* argv[]) {
    // expects an additional command-level argument referencing path to the clear script to run
    if (argc != 2) {
        std::cerr << "Incorrect call" << std::endl;
        std::cerr << "Example: ./clear <../example_script.clr>" << std::endl;
        return EXIT_FAILURE;
    }
    
    // read entire source file into a stream, then into a string
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in); 
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));
    // convert source string to tokens using the tokenize function
    std::vector<Token> tokens = tokenizer.tokenize();

    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}