cmake_minimum_required(VERSION 3.1)

project(learn-opengl)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    GL
    GLEW
    glfw
)