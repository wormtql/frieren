add_library(frieren_imgui_impl)

file(GLOB imgui_headers ${PROJECT_SOURCE_DIR}/3rd/imgui/*.h)
file(GLOB imgui_sources ${PROJECT_SOURCE_DIR}/3rd/imgui/*.cpp)

target_link_libraries(frieren_imgui_impl glfw webgpu)

#message(${imgui_headers})

target_sources(frieren_imgui_impl PRIVATE ${imgui_sources})
target_sources(frieren_imgui_impl PUBLIC ${imgui_headers})
#target_sources(frieren_imgui_impl PRIVATE ${PROJECT_SOURCE_DIR}/3rd/imgui/*.cpp)
#target_sources(frieren_imgui_impl PUBLIC ${PROJECT_SOURCE_DIR}/3rd/imgui/*.h)
target_sources(frieren_imgui_impl PRIVATE ${PROJECT_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_wgpu.cpp)
target_sources(frieren_imgui_impl PUBLIC ${PROJECT_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_wgpu.h)
target_sources(frieren_imgui_impl PRIVATE ${PROJECT_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.cpp)
target_sources(frieren_imgui_impl PUBLIC ${PROJECT_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.h)
target_sources(frieren_imgui_impl PRIVATE ${PROJECT_SOURCE_DIR}/3rd/imgui/misc/cpp/imgui_stdlib.h)
target_sources(frieren_imgui_impl PUBLIC ${PROJECT_SOURCE_DIR}/3rd/imgui/misc/cpp/imgui_stdlib.cpp)

target_include_directories(frieren_imgui_impl PUBLIC ${PROJECT_SOURCE_DIR}/3rd/imgui)
