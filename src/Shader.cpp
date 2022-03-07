#include "Shader.h"

#include <fstream>                      // std::ifstream
#include <sstream>                      // std::stringstream
#include <string>                       // std::string

#include <GL/glew.h>                    // glCreateShader

#include "Renderer.h"                   // SNBL_GLCALL

namespace snbl {

std::string readShader(const std::string &path) {
    std::ifstream file(path);
    std::string line;
    std::stringstream source;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            source << line << '\n';
        }
    } else {
        std::printf("Failed to read shader: file '%s' not found\n",
            path.c_str());
    }

    return source.str();
}

ShaderSource readShaderVF(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexSource = readShader(vertexPath);
    std::string fragmentSource = readShader(fragmentPath);

    return { vertexSource, fragmentSource };
}

unsigned int compileShader(unsigned int type, const std::string& source) {
    SNBL_GLCALL(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    SNBL_GLCALL(glShaderSource(id, 1, &src, nullptr));
    SNBL_GLCALL(glCompileShader(id));

    int result;
    SNBL_GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        SNBL_GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        SNBL_GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::printf("Failed to compile shader: %s\n", message);

        SNBL_GLCALL(glDeleteShader(id));

        return 0;
    }

    return id;
}

unsigned int createProgram(const std::string& vertexSource, const std::string& fragmentSource) {
    SNBL_GLCALL(unsigned int program = glCreateProgram());
    SNBL_GLCALL(unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSource));
    SNBL_GLCALL(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource));

    SNBL_GLCALL(glAttachShader(program, vs));
    SNBL_GLCALL(glAttachShader(program, fs));
    SNBL_GLCALL(glLinkProgram(program));
    SNBL_GLCALL(glValidateProgram(program));

    SNBL_GLCALL(glDeleteShader(vs));
    SNBL_GLCALL(glDeleteShader(fs));

    return program;
}

} // namespace snbl