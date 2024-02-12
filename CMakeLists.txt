cmake_minimum_required(VERSION 3.20)

project(
    MSHParser
    VERSION 0.1.0
    LANGUAGES CXX
)

add_subdirectory(lib)
add_subdirectory(src)

target_link_libraries(main PUBLIC mshreader)
