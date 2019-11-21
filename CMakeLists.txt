cmake_minimum_required(VERSION 3.10)

if(WIN32)
    add_compile_options("/std:c++latest")
endif(WIN32)

project(SoftwareGL)

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_library(software_gl STATIC
    ${PROJECT_SOURCE_DIR}/software_gl/Mesh.h
    ${PROJECT_SOURCE_DIR}/software_gl/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Camera.h
    ${PROJECT_SOURCE_DIR}/software_gl/Camera.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Triangle.h
    ${PROJECT_SOURCE_DIR}/software_gl/Triangle.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Image.h
    ${PROJECT_SOURCE_DIR}/software_gl/Image.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Vertex.h
    ${PROJECT_SOURCE_DIR}/software_gl/VectorMath.h
    ${PROJECT_SOURCE_DIR}/software_gl/VectorMath.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Renderer.h
    ${PROJECT_SOURCE_DIR}/software_gl/Renderer.cpp
)

target_link_libraries(software_gl PRIVATE
    SDL2::SDL2
    GLEW::GLEW
)

add_executable(software WIN32
    ${PROJECT_SOURCE_DIR}/software/main.cpp
    ${PROJECT_SOURCE_DIR}/software/WindowInterface.h
    ${PROJECT_SOURCE_DIR}/software/WindowSoftwareGL.h
    ${PROJECT_SOURCE_DIR}/software/WindowSoftwareGL.cpp
    ${PROJECT_SOURCE_DIR}/software/WindowSDL2GL.cpp
    ${PROJECT_SOURCE_DIR}/software/WindowSDL2GL.h
)

target_link_libraries(software PRIVATE
    software_gl
    SDL2::SDL2
    GLEW::GLEW
)

add_executable(gl_shader WIN32
    ${PROJECT_SOURCE_DIR}/gl_shader/main.cpp
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowInterface.h
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSoftwareGL.h
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSoftwareGL.cpp
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSDL2GL.cpp
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSDL2GL.h
)

target_link_libraries(gl_shader PRIVATE
    software_gl
    SDL2::SDL2
    GLEW::GLEW
)
