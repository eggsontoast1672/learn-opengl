#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    float positions[6] = {
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f
    };

    /* Create a vertex buffer and put data inside it */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}