add_library(frieren_application)
target_link_libraries(frieren_application webgpu webgpu_impl nlohmann_json::nlohmann_json glm stb_image)
target_link_libraries(frieren_application glfw glfw3webgpu nanoid frieren_imgui_impl)
target_link_libraries(frieren_application frieren_core frieren_editor)

file(GLOB_RECURSE frieren_application_headers "*.h")
file(GLOB_RECURSE frieren_application_sources "*.cpp")

target_include_directories(frieren_application PUBLIC .)
target_sources(frieren_application PRIVATE
        ${frieren_application_sources}
        )
target_sources(frieren_application PRIVATE ${frieren_application_headers})
