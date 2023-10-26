add_library(frieren_core)
target_link_libraries(frieren_core webgpu webgpu_impl nlohmann_json::nlohmann_json)

file(GLOB frieren_core_headers "**/*.h" "*.h")
file(GLOB frieren_core_sources "**/*.cpp" "*.cpp")

target_include_directories(frieren_core PUBLIC .)
target_sources(frieren_core PRIVATE
        ${frieren_core_sources}
        )
target_sources(frieren_core PUBLIC ${frieren_core_headers})
