#ifndef GENERATION_UTILS_HPP_INCLUDED
#define GENERATION_UTILS_HPP_INCLUDED 0

#include <string>
#include <iostream>
#include <fstream>

std::string get_include_guard_str (const std::string &destination_filepath);

void write_include_guard_start (
    std::ofstream &out, const std::string &include_guard_str
);

void write_include_guard_end (
    std::ofstream &out, const std::string &include_guard_str
);

std::ofstream& write_indent (std::ofstream &out, unsigned indent);

#endif // GENERATION_UTILS_HPP_INCLUDED
