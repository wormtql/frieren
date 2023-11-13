add_library(frieren_editor)
target_link_libraries(frieren_editor webgpu webgpu_impl nlohmann_json::nlohmann_json glm stb_image)
target_link_libraries(frieren_editor glfw glfw3webgpu nanoid frieren_imgui_impl frieren_core)

file(GLOB_RECURSE frieren_editor_headers "*.h")
file(GLOB_RECURSE frieren_editor_sources "*.cpp")

target_include_directories(frieren_editor PUBLIC .)
target_sources(frieren_editor PRIVATE
        ${frieren_editor_sources}
        )
target_sources(frieren_editor PRIVATE ${frieren_editor_headers})
