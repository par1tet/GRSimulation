cmake_minimum_required(VERSION 3.18)
project(grSim VERSION 0.1.0)

add_executable(grSim
    src/main.cpp
    src/utils/shader_utils.cpp
    src/mesh/sphereMesh.cpp
    src/kinematics/body.cpp
    src/classes/camera.cpp
)

include(FetchContent)
FetchContent_Declare(soil2  
    GIT_REPOSITORY https://github.com/SpartanJ/SOIL2  
    GIT_TAG master  
)  
FetchContent_MakeAvailable(soil2)

add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

target_include_directories(grSim PRIVATE
    ${CMAKE_SOURCE_DIR}/core
)

target_include_directories(grSim PRIVATE  
    ${soil2_SOURCE_DIR}/src  
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(grSim PRIVATE soil2 glad glfw OpenGL::GL glm::glm)
target_compile_definitions(grSim PRIVATE CMAKE_SOURCE_PATH="${CMAKE_SOURCE_DIR}/")