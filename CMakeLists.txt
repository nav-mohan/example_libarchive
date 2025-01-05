cmake_minimum_required(VERSION 3.11)
project(ExampleLibArchive LANGUAGES CXX C)
set (CMAKE_CXX_STANDARD 17)

set(TARGET_NAME ${PROJECT_NAME})

# enable/disable required libarchive FLAGS 
include(${CMAKE_CURRENT_SOURCE_DIR}/libarchive_flags.cmake)

# Add libarchive as a subdirectory
add_subdirectory(third_party/libarchive)

# Include libarchive headers
include_directories(third_party/libarchive/libarchive)

# Add your executable
add_executable(${TARGET_NAME} main.c)

# target_link_libraries(${TARGET_NAME} archive)
target_link_libraries(${TARGET_NAME} archive_static)
