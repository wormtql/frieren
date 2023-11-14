add_library(frieren_core)
target_link_libraries(frieren_core webgpu webgpu_impl nlohmann_json::nlohmann_json glm stb_image)
target_link_libraries(frieren_core glfw glfw3webgpu nanoid frieren_imgui_impl)
target_link_libraries(frieren_core assimp::assimp)

file(GLOB_RECURSE frieren_core_headers "*.h")
file(GLOB_RECURSE frieren_core_sources "*.cpp")

target_include_directories(frieren_core PUBLIC .)
target_sources(frieren_core PRIVATE
        ${frieren_core_sources}
        )
target_sources(frieren_core PUBLIC ${frieren_core_headers})
