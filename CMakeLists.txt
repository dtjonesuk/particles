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

file(GLOB SHADER_SRCS 
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.glsl
)
message(${SHADER_SRCS})

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
        include/framework/Camera.h
        include/framework/Wireframe.h
        include/framework/StaticMesh.h
        include/glutils.h
        include/DemoWindow.h
        src/glutils.cpp
        src/framework/Window.cpp
        src/DemoWindow.cpp

        
        ${SHADER_SRCS}
        include/framework/Geometry.h
        include/framework/Transform.h
        include/framework/Light.h
        include/framework/MaterialUniform.h
        include/framework/Buffer.h
        include/framework/MatrixUniform.h
        include/framework/Scene.h
        include/framework/Material.h
        include/framework/Entity.h
        include/framework/Uniforms.h
        include/framework/Particle.h
        include/framework/InstancedMesh.h
        include/framework/GlNamedResource.h
        src/framework/Shader.cpp
        include/framework/Framework.h
        include/framework/SolidMaterialRenderer.h
        include/framework/WireframeRenderer.h
        include/framework/Renderer.h
)

target_link_libraries(particles PRIVATE glm::glm)
target_link_libraries(particles PRIVATE glfw)


foreach(SHADER ${SHADER_SRCS})
set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
endforeach()

add_custom_command(TARGET particles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"
)