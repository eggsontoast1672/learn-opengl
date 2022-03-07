cmake_minimum_required(VERSION 3.1)

project(learn-opengl)

add_executable(
    main
    src/IndexBuffer.cpp
    src/Main.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/VertexBuffer.cpp
)

target_include_directories(
    main
    PRIVATE
    include/
)

target_link_libraries(
    main
    GL
    GLEW
    glfw
)