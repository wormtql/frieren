add_library(frieren_precompute)
target_link_libraries(frieren_precompute nlohmann_json::nlohmann_json glm stb_image stb_image_write)
target_link_libraries(frieren_precompute frieren_core)

file(GLOB_RECURSE frieren_precompute_headers "*.h")
file(GLOB_RECURSE frieren_precompute_sources "*.cpp")

target_include_directories(frieren_precompute PUBLIC .)
target_sources(frieren_precompute PRIVATE
        ${frieren_precompute_sources}
        )
target_sources(frieren_precompute PUBLIC ${frieren_precompute_headers})

add_subdirectory(bin)
