cmake_minimum_required(VERSION 3.16)
project(VoxMine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


find_package(OpenGL REQUIRED)


FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.9
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)




add_executable(VoxMine
    src/main.cpp
)


target_link_libraries(VoxMine
    PRIVATE
        OpenGL::GL
        glfw
        glm::glm
)

# if (MSVC)
#     target_compile_options(VoxMine PRIVATE /W4)
# else()
#     target_compile_options(VoxMine PRIVATE -Wall -Wextra -Wpedantic)
# endif()
