cmake_minimum_required(VERSION 3.31)
project(particles)

include(FetchContent)

#fetch glm
FetchContent_Declare(glm
        GIT_REPOSITORY  https://github.com/g-truc/glm.git
        GIT_TAG         1.0.1
)
FetchContent_MakeAvailable(glm)

#fetch glfw
FetchContent_Declare(glfw3
        GIT_REPOSITORY  https://github.com/glfw/glfw.git
        GIT_TAG         3.4
)
FetchContent_MakeAvailable(glfw3)


set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(extern/glad/include)
include_directories(extern/stb)
include_directories(extern/imgui)

add_executable(particles src/main.cpp extern/glad/src/glad.c
        extern/imgui/imgui.cpp
        extern/imgui/imgui_draw.cpp
        extern/imgui/imgui_tables.cpp
        extern/imgui/imgui_widgets.cpp
        extern/imgui/imgui_demo.cpp
        extern/imgui/backends/imgui_impl_glfw.cpp
        extern/imgui/backends/imgui_impl_opengl3.cpp
        include/framework/Window.h
        include/framework/Shader.h
        include/framework/ShaderProgram.h
        include/framework/GlResource.h
        include/framework/Mesh.h
        include/framework/Texture.h
        include/glutils.h
        include/DemoWindow.h
        src/glutils.cpp
        src/framework/Window.cpp
        src/DemoWindow.cpp
)

target_link_libraries(particles PRIVATE glm::glm)
target_link_libraries(particles PRIVATE glfw)

set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/2d.frag.glsl)
set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/2d.vert.glsl)
set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/3d.frag.glsl)
set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/3d.vert.glsl)

add_custom_command(TARGET particles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"
)
#file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})