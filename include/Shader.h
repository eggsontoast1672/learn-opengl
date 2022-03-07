#pragma once

#include <string>                       // std::string

namespace snbl {

struct ShaderSource {
    std::string vertex;
    std::string fragment;
};

std::string readShader(const std::string &);
ShaderSource readShaderVF(const std::string &, const std::string &);

unsigned int compileShader(unsigned int, const std::string &);
unsigned int createProgram(const std::string &, const std::string &);

} // namespace snbl