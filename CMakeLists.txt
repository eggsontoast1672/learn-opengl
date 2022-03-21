cmake_minimum_required(VERSION 3.1)

project(learn-opengl)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -Wpedantic)
endif()

add_executable(
    main
    src/main.c    
)

set_property(TARGET main PROPERTY C_STANDARD 90)

# target_include_directories(
#     main
#     PRIVATE
#     inc/
# )

target_link_libraries(
    main
    glfw
    GL
    GLEW
)
