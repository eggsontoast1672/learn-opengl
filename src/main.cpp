#include <signal.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SNBL_ASSERT(x) \
    if (!(x)) \
        raise(SIGTRAP)

#define SNBL_CALL(x) \
    snbl_clearError(); \
    x; \
    SNBL_ASSERT(snbl_logCall(__FILE__, __LINE__, #x))

struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
};

static void snbl_clearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool snbl_logCall(const char *file, int line, const char *function) {
    GLenum error;
    while (error = glGetError()) {
        std::cout << "File: " << file << '\n'
                  << "Line: " << line << '\n'
                  << "Function: " << function << '\n'
                  << "Error: " << error << std::endl;
        return false;
    }
    return true;
}

static ShaderProgramSource snbl_parseShader(const std::string& file) {
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

static unsigned int snbl_compileShader(unsigned int type, const std::string& source) {
    SNBL_CALL(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    SNBL_CALL(glShaderSource(id, 1, &src, nullptr));
    SNBL_CALL(glCompileShader(id));

    // TODO: Handle shader compilation errors
    int result;
    SNBL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        SNBL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        SNBL_CALL(glGetShaderInfoLog(id, length, &length, message));

        const char *typeFmt;
        if (type == GL_VERTEX_SHADER) typeFmt = "vertex";
        else if (type == GL_FRAGMENT_SHADER) typeFmt = "fragment";
        std::cout << "Failed to compile " << typeFmt << " shader: " << message << std::endl;

        SNBL_CALL(glDeleteShader(id));

        return 0;
    }

    return id;
}

static unsigned int snbl_createShader(const std::string& vertexSource, const std::string& fragmentSource) {
    SNBL_CALL(unsigned int program = glCreateProgram());
    SNBL_CALL(unsigned int vs = snbl_compileShader(GL_VERTEX_SHADER, vertexSource));
    SNBL_CALL(unsigned int fs = snbl_compileShader(GL_FRAGMENT_SHADER, fragmentSource));

    SNBL_CALL(glAttachShader(program, vs));
    SNBL_CALL(glAttachShader(program, fs));
    SNBL_CALL(glLinkProgram(program));
    SNBL_CALL(glValidateProgram(program));

    SNBL_CALL(glDeleteShader(vs));
    SNBL_CALL(glDeleteShader(fs));

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
    glfwSwapInterval(1);

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

    /* Create a vertex buffer and put data inside it */
    unsigned int buffer;
    SNBL_CALL(glGenBuffers(1, &buffer));
    SNBL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    SNBL_CALL(glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW));

    SNBL_CALL(glEnableVertexAttribArray(0));
    SNBL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    unsigned int ibo;
    SNBL_CALL(glGenBuffers(1, &ibo));
    SNBL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    SNBL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = snbl_parseShader("../res/shaders/basic.glsl");
    unsigned int shader = snbl_createShader(source.vertex, source.fragment);
    SNBL_CALL(glUseProgram(shader));

    SNBL_CALL(int location = glGetUniformLocation(shader, "u_color"));
    SNBL_ASSERT(location != -1);

    float r = 0.0f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        SNBL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        /* Draw the square */
        SNBL_CALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        SNBL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Animate the color of the rectangle */
        if (r > 1.0f || r < 0.0f)
            increment *= -1;
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    SNBL_CALL(glDeleteProgram(shader));
    glfwTerminate();

    return 0;
}