cmake_minimum_required(VERSION 3.10)

project(SoftwareGL)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_library(SoftwareGL STATIC
    ${PROJECT_SOURCE_DIR}/software_gl/ImageInterface.h
    ${PROJECT_SOURCE_DIR}/software_gl/WindowInterface.h
    ${PROJECT_SOURCE_DIR}/software_gl/WindowSDL2GL.cpp
    ${PROJECT_SOURCE_DIR}/software_gl/WindowSDL2GL.h
)

target_link_libraries(SoftwareGL PRIVATE
    SDL2::SDL2
    GLEW::GLEW
)

add_executable(Game
    ${PROJECT_SOURCE_DIR}/example/main.cpp
    ${PROJECT_SOURCE_DIR}/example/WindowSoftwareGL.h
)

target_link_libraries(Game PRIVATE
    SoftwareGL
    SDL2::SDL2
    GLEW::GLEW
)
