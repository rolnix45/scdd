#pragma once

#include <prerequisites.hpp>

#include <vector>
#include <optional>
#include <string>

class SCDDMain {
public:
    SCDDMain(s32 argc, char* argv[]);
private:
    std::string m_input_file_name;
    std::string m_output_file_name;

    std::string m_file_content;
    u8 read_file(std::string const &filename);
    u8 output_file(std::string const &code, std::string const &filename);

    s32 get_index(std::vector<std::string> const &vector, std::string const &key);
};