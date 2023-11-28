add_library(stb_image stb_image_impl.cpp)
target_include_directories(stb_image PUBLIC ${CMAKE_SOURCE_DIR}/3rd/stb)

add_library(stb_image_write stb_image_write_impl.cpp)
target_include_directories(stb_image_write PUBLIC ${CMAKE_SOURCE_DIR}/3rd/stb)
