cmake_minimum_required(VERSION 3.10)

if(WIN32)
    add_compile_options("/std:c++latest")
endif(WIN32)
set(CMAKE_CXX_STANDARD 17)

project(SoftwareGL)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GLEW REQUIRED)

enable_testing()
find_package(Gtest CONFIG REQUIRED)

add_library(software_gl
  STATIC
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
    ${PROJECT_SOURCE_DIR}/software_gl/Scene.h
    ${PROJECT_SOURCE_DIR}/software_gl/Scene.cpp
)

if (NOT APPLE)
    target_link_libraries(software_gl 
      PRIVATE
        SDL2::SDL2
        GLEW::GLEW
        imgui::imgui
    )
else()
    target_link_libraries(software_gl
      PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        GLEW::GLEW
        imgui::imgui
    )
endif()

add_executable(software_gl_test
    ${PROJECT_SOURCE_DIR}/software_gl_test/main.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Mesh_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Mesh_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Camera_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Camera_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Triangle_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Triangle_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Image_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Image_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Vertex_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Vertex_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/VectorMath_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/VectorMath_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Renderer_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Renderer_test.cpp
    ${PROJECT_SOURCE_DIR}/software_gl_test/Scene_test.h
    ${PROJECT_SOURCE_DIR}/software_gl_test/Scene_test.cpp
)

add_test(
  NAME
    software_gl_test
  COMMAND
    software_gl_test
)

target_link_libraries(software_gl_test
  PUBLIC
    open_gl
    software_gl
  PRIVATE
    GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
    SDL2::SDL2
    GLEW::GLEW
)

add_library(open_gl
  STATIC
    ${PROJECT_SOURCE_DIR}/open_gl/Shader.h
    ${PROJECT_SOURCE_DIR}/open_gl/Shader.cpp
    ${PROJECT_SOURCE_DIR}/open_gl/Program.h
    ${PROJECT_SOURCE_DIR}/open_gl/Program.cpp
    ${PROJECT_SOURCE_DIR}/open_gl/Texture.h
    ${PROJECT_SOURCE_DIR}/open_gl/Texture.cpp
    ${PROJECT_SOURCE_DIR}/open_gl/Device.h
    ${PROJECT_SOURCE_DIR}/open_gl/Device.cpp
    ${PROJECT_SOURCE_DIR}/open_gl/Mesh.h
    ${PROJECT_SOURCE_DIR}/open_gl/Mesh.cpp
)

if (NOT APPLE)
    target_link_libraries(open_gl
      PRIVATE
        SDL2::SDL2
        GLEW::GLEW
        imgui::imgui
    )
else()
    target_link_libraries(open_gl
      PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        GLEW::GLEW
        imgui::imgui
    )
endif()

add_executable(open_gl_test
    ${PROJECT_SOURCE_DIR}/open_gl_test/main.cpp
    ${PROJECT_SOURCE_DIR}/open_gl_test/Shader_test.h
    ${PROJECT_SOURCE_DIR}/open_gl_test/Shader_test.cpp
    ${PROJECT_SOURCE_DIR}/open_gl_test/Program_test.h
    ${PROJECT_SOURCE_DIR}/open_gl_test/Program_test.cpp
    ${PROJECT_SOURCE_DIR}/open_gl_test/Texture_test.h
    ${PROJECT_SOURCE_DIR}/open_gl_test/Texture_test.cpp
    ${PROJECT_SOURCE_DIR}/open_gl_test/Device_test.h
    ${PROJECT_SOURCE_DIR}/open_gl_test/Device_test.cpp
    ${PROJECT_SOURCE_DIR}/open_gl_test/Mesh_test.h
    ${PROJECT_SOURCE_DIR}/open_gl_test/Mesh_test.cpp
)

add_test(
  NAME
    open_gl_test
  COMMAND
    open_gl_test
)

target_link_libraries(open_gl_test
  PUBLIC
    open_gl
  PRIVATE
    GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
    SDL2::SDL2
    GLEW::GLEW
)

add_library(imgui_impl
  STATIC
    ${PROJECT_SOURCE_DIR}/imgui/imgui_impl_sdl.h
    ${PROJECT_SOURCE_DIR}/imgui/imgui_impl_sdl.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_impl_opengl3.h
    ${PROJECT_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp
)

if (NOT APPLE)
    target_link_libraries(imgui_impl 
      PRIVATE
        SDL2::SDL2
        GLEW::GLEW
        imgui::imgui
    )
else()
    target_link_libraries(imgui_impl 
      PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        GLEW::GLEW
        imgui::imgui
    )
endif()

add_executable(software 
  WIN32
    ${PROJECT_SOURCE_DIR}/software/main.cpp
    ${PROJECT_SOURCE_DIR}/software/WindowInterface.h
    ${PROJECT_SOURCE_DIR}/software/WindowSoftwareGL.h
    ${PROJECT_SOURCE_DIR}/software/WindowSoftwareGL.cpp
    ${PROJECT_SOURCE_DIR}/software/WindowSDL2GL.cpp
    ${PROJECT_SOURCE_DIR}/software/WindowSDL2GL.h
)

if (NOT APPLE)
    target_link_libraries(software 
      PUBLIC
        software_gl
      PRIVATE
        SDL2::SDL2
        GLEW::GLEW
        imgui::imgui
    )
else()
    target_link_libraries(software 
      PUBLIC
        software_gl
      PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        GLEW::GLEW
        imgui::imgui
    )
endif()

add_executable(gl_shader 
  WIN32
    ${PROJECT_SOURCE_DIR}/gl_shader/main.cpp
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowInterface.h
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSoftwareGL.h
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSoftwareGL.cpp
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSDL2GL.cpp
    ${PROJECT_SOURCE_DIR}/gl_shader/WindowSDL2GL.h
	${PROJECT_SOURCE_DIR}/asset/vertex.glsl
	${PROJECT_SOURCE_DIR}/asset/fragment.glsl
)

if (NOT APPLE)
    target_link_libraries(gl_shader 
      PUBLIC
        open_gl
        software_gl
        imgui_impl
      PRIVATE
        SDL2::SDL2
        GLEW::GLEW
        imgui::imgui
    )
else()
    target_link_libraries(gl_shader 
      PUBLIC
        open_gl
        software_gl
        imgui_impl
      PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        GLEW::GLEW
        imgui::imgui
)
endif()
