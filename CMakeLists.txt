cmake_minimum_required(VERSION 3.10)

project(SoftwareGL)

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_library(SoftwareGL STATIC
    ${PROJECT_SOURCE_DIR}/software_gl/Mesh.h
    ${PROJECT_SOURCE_DIR}/software_gl/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Camera.h
    ${PROJECT_SOURCE_DIR}/software_gl/Camera.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Triangle.h
    ${PROJECT_SOURCE_DIR}/software_gl/Triangle.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/Image.h
    ${PROJECT_SOURCE_DIR}/software_gl/Image.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/ImageInterface.h
    ${PROJECT_SOURCE_DIR}/software_gl/Vertex.h
    ${PROJECT_SOURCE_DIR}/software_gl/WindowInterface.h
    ${PROJECT_SOURCE_DIR}/software_gl/WindowSDL2GL.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/WindowSDL2GL.h
    ${PROJECT_SOURCE_DIR}/software_gl/VectorMath.h
    ${PROJECT_SOURCE_DIR}/software_gl/VectorMath.cpp
)

target_link_libraries(SoftwareGL PRIVATE
    SDL2::SDL2
    GLEW::GLEW
)

add_executable(Game WIN32
    ${PROJECT_SOURCE_DIR}/example/main.cpp
    ${PROJECT_SOURCE_DIR}/example/WindowSoftwareGL.h
    ${PROJECT_SOURCE_DIR}/example/WindowSoftwareGL.cpp
)

target_link_libraries(Game PRIVATE
    SoftwareGL
    SDL2::SDL2
    GLEW::GLEW
)
