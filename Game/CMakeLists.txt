cmake_minimum_required(VERSION 3.20.0)

project("RTGame")

file(GLOB_RECURSE RTG_HEADERS "src/*.cpp")
file(GLOB_RECURSE RTG_SOURCES "src/*.h")

add_executable (rtgame ${RTG_SOURCES} ${RTG_HEADERS})

set_property(TARGET rtgame PROPERTY CXX_STANDARD 20)

# ---- Libraries ----
# NetNT
add_subdirectory("vendor/NetNT" ${CMAKE_CURRENT_BINARY_DIR}/NetNT)
target_link_libraries(rtgame NetNT)
target_include_directories(rtgame PUBLIC "vendor/NetNT/src/")

# RTEngine
target_link_libraries(rtgame rtengine)
