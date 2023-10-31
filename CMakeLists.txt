cmake_minimum_required(VERSION 3.20)
project(frieren)

set(CMAKE_CXX_STANDARD 23)

# glfw
add_subdirectory(3rd/glfw)

# glm
add_subdirectory(3rd/glm)

# wgpu
add_subdirectory(3rd/webgpu-distribution)
add_subdirectory(3rd/glfw3webgpu)
include_directories(3rd/webgpu-distribution/include)

# nlohmann json
add_subdirectory(3rd/nlohmann_json)

add_executable(frieren main.cpp webgpu_cpp_impl.cpp)
target_link_libraries(frieren glfw webgpu glfw3webgpu)
target_copy_webgpu_binaries(frieren)

add_subdirectory(src)
