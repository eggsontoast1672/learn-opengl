#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define VERTICES_SIZE   6

GLFWwindow *window;

float vertices[VERTICES_SIZE] = {
    -0.5, -0.5,
     0.0,  0.5,
     0.5, -0.5
};

unsigned vao, vbo;

void init(void);
void update(void);

int main(void) {
    if (glfwInit() != GLFW_TRUE) {
        printf("Failed to initialize glfw\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "learn-opengl", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize glew");
        glfwTerminate();
        return 1;
    }

    init();

    while (!glfwWindowShouldClose(window)) {
        update(); 
    }

    glfwTerminate();

    return 0;
}


void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
}

void update(void) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
}
