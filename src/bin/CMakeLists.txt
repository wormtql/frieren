add_executable(test1 test1.cpp)
target_link_libraries(test1 frieren_core)
target_copy_webgpu_binaries(test1)