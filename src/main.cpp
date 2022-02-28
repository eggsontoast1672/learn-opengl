#include <signal.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
};

static ShaderProgramSource ParseShader(const std::string& file) {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(file);
    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Handle shader compilation errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);

        const char *typeFmt;
        if (type == GL_VERTEX_SHADER) typeFmt = "vertex";
        else if (type == GL_FRAGMENT_SHADER) typeFmt = "fragment";
        std::cout << "Failed to compile " << typeFmt << " shader: " << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void) {
    if (glfwInit() != GLFW_TRUE) {
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    /* Debug output */
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::string vertexSource =
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;"
        "void main() {"
        "gl_Position = position;"
        "}";
    std::string fragmentSource =
        "#version 330 core\n"
        "layout(location = 0) out vec4 color;"
        "void main() {"
        "color = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
        "}";

    ShaderProgramSource source = ParseShader("../res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.vertex, source.fragment);
    glUseProgram(shader);

    /* Create a vertex buffer and put data inside it */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();

    raise(SIGTRAP);

    return 0;
}