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

add_executable(particles src/main.cpp extern/glad/src/glad.c)

target_link_libraries(particles PRIVATE glm::glm)
target_link_libraries(particles PRIVATE glfw)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})