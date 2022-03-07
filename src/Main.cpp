#include <signal.h>

#include <string>                       // std::string

#include <GL/glew.h>                    // glewInit
#include <GLFW/glfw3.h>                 // glfwInit

#include "IndexBuffer.h"                // snbl::IndexBuffer
#include "Renderer.h"                   // SNBL_GLCALL
#include "Shader.h"                     // snbl::readShaderVF
#include "VertexBuffer.h"               // snbl::VertexBuffer

int main() {
    if (glfwInit() != GLFW_TRUE) {
        std::printf("Failed to initialize glfw\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == NULL) {
        std::printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::printf("Failed to initialize glew\n");
        glfwTerminate();
        return -1;
    }

    std::printf("OpenGL version: %s\n", glGetString(GL_VERSION));

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

    /* Create a vertex buffer and put data inside it */
    snbl::VertexBuffer vb(12 * sizeof(float), positions);

    SNBL_GLCALL(glEnableVertexAttribArray(0));
    SNBL_GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    snbl::IndexBuffer ib(6, indices);

    snbl::ShaderSource source = snbl::readShaderVF(
        "../res/shaders/basic.vert",
        "../res/shaders/basic.frag"
    );

    unsigned int shader = snbl::createProgram(source.vertex, source.fragment);
    SNBL_GLCALL(glUseProgram(shader));

    SNBL_GLCALL(int location = glGetUniformLocation(shader, "u_color"));
    SNBL_ASSERT(location != -1);

    float r = 0.0f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window)) {
        SNBL_GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        SNBL_GLCALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        SNBL_GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f || r < 0.0f)
            increment *= -1;
        r += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    SNBL_GLCALL(glDeleteProgram(shader));
    glfwTerminate();

    return 0;
}