cmake_minimum_required(VERSION 3.0)
project(file_walker)
set(CMAKE_CXX_STANDARD 17)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>")
add_compile_options("$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>")

aux_source_directory(src SRC)
find_package(Threads REQUIRED)

add_executable(sample ${SRC})
target_link_libraries(sample ${CMAKE_THREAD_LIBS_INIT})
