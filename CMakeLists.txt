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

add_executable(particles src/main.cpp extern/glad/src/glad.c
        src/glutils.cpp
        include/glutils.h
        src/Window.cpp
        include/framework/Window.h
        src/Shader.cpp
        include/framework/Shader.h
        src/ShaderProgram.cpp
        include/framework/ShaderProgram.h
        src/DemoWindow.cpp
        include/DemoWindow.h
        src/GLResource.cpp
        include/framework/Resource.h
)

target_link_libraries(particles PRIVATE glm::glm)
target_link_libraries(particles PRIVATE glfw)

set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/2d.frag.glsl)
set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/2d.vert.glsl)

add_custom_command(TARGET particles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"
)
#file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})