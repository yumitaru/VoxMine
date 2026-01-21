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
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.36
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.3.1
)

# Assimp options – wyłączamy rzeczy których nie potrzebujesz
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

# Konfiguracja GLAD
set(GLAD_PROFILE "core")
set(GLAD_API "gl=3.3")     # możesz zmienić np. na gl=3.3
set(GLAD_GENERATOR "c")
set(GLAD_SPEC "gl")

FetchContent_MakeAvailable(glad)




add_executable(${PROJECT_NAME}
    ${stb_SOURCE_DIR} 
    src/main.cpp
    src/Core/Application.cpp
    src/Core/Camera.cpp
    src/Graphics/Shader.cpp
    src/Graphics/Renderer.cpp
    src/Graphics/HUDRenderer.cpp
    src/Graphics/ChestRenderer.cpp
    src/Graphics/Model.cpp
    src/World/Floor.cpp
    src/WorldGeneration/World.cpp
    src/WorldGeneration/RandomIntGen.cpp
    src/WorldGeneration/Block.hpp
    src/WorldGeneration/WorldManager.cpp
    src/Input/Input.cpp
    src/Graphics/HandRenderer.cpp
    src/Player/Player.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${stb_SOURCE_DIR}
)


target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        glm::glm
        glad
        assimp
)

# if (MSVC)
#     target_compile_options(VoxMine PRIVATE /W4)
# else()
#     target_compile_options(VoxMine PRIVATE -Wall -Wextra -Wpedantic)
# endif()
