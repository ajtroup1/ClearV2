// TODO: 8:21 pt.2

#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "parser.hpp"
#include "tokenization.hpp"
#include "generation.hpp"
#include "arena.hpp"

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

    Parser parser(std::move(tokens));
    std::optional<NodeProgram> prog = parser.parse_prog();

    if (!prog.has_value()) {
        std::cerr << "Unable to parse tokens" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(prog.value());

    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.generate_prog();
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}