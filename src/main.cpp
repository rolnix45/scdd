#include <main.hpp>
#include <tokenizer.hpp>
#include <parser.hpp>
#include <generator.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>

char* compiler_exec;

SCDDMain::SCDDMain(s32 argc, char* argv[]) {
    // generate string vector out of arguments
    std::vector<std::string> args;
    for (s32 i = 0; i < argc; i++)
        args.push_back(std::string(argv[i]));

    // get compiler execution command
    compiler_exec = args.at(0).data();

    // get and read source code
    if (s32 index = get_index(args, "-i")) {
        std::cout << "compiling: " << args.at(index + 1) << '\n';
        m_input_file_name = args.at(index + 1);
    } else {
        fatal_error("File not found!"); 
    }
    if (!read_file(m_input_file_name)) {
        fatal_error("Given file could not be read!");
    }

    // compile
    SCDDTokenizer tokenizer(std::move(m_file_content));
    std::vector<Token> tokens = tokenizer.tokenize();
    SCDDParser parser(std::move(tokens));
    std::optional<node::prog> tree = parser.parse_program();
    if (!tree.has_value()) {
        fatal_error("invalid program");
    }

    // output assembly
    SCDDGenerator generator(tree.value());
    std::string code = generator.gen_prog();
    if (s32 index = get_index(args, "-o")) {
        std::cout << "outputing to: " << args.at(index + 1) << '\n';
        m_output_file_name = args.at(index + 1);
    } else { 
        m_output_file_name = "out.s"; 
    }
    if (!output_file(code, m_output_file_name)) {
        fatal_error("Could not output an assembly file!"); 
    }
}

u8 SCDDMain::read_file(std::string const &filename) {
    std::ifstream file(filename);
    if (!file.good()) return false;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    m_file_content = buffer.str();
    return true;
}

u8 SCDDMain::output_file(std::string const &code, std::string const &filename) {
    std::ofstream file(filename);
    if (!file.good()) return false;
    file << code;
    file.close();
    return true;
}

s32 SCDDMain::get_index(std::vector<std::string> const &vector, std::string const &key) {
    std::vector<std::string>::const_iterator it = std::find(vector.begin(), vector.end(), key);
    if (it == vector.end()) return false;
    return it - vector.begin();
}

int main(s32 argc, char* argv[]) {
    try {
        SCDDMain main(argc, argv);
    } catch (std::exception const &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}