cmake_minimum_required(VERSION 3.20.0)

project("RTEngine")

file(GLOB_RECURSE RTE_SOURCES "src/*.cpp")
file(GLOB_RECURSE RTE_HEADERS "src/*.h")
file(GLOB_RECURSE PLATFORM_WINDOWS_SOURCES "src/Platform/Windows/*.cpp")
#file(GLOB_RECURSE PLATFORM_LINUX_SOURCES "src/Platform/Linux/*.cpp")
file(GLOB_RECURSE PLATFORM_POSIX_SOURCES "src/Platform/POSIX/*.cpp")
#file(GLOB_RECURSE PLATFORM_OSX_SOURCES "src/Platform/OSX/*.cpp")

add_library (rtengine ${RTE_SOURCES} ${RTE_HEADERS} "src/Renderer/Renderer.cpp")

set_property(TARGET rtengine PROPERTY CXX_STANDARD 20)

target_include_directories(rtengine PUBLIC "src")

if(CMAKE_SYSNAME STREQUAL "Windows")
	target_sources(rtengine PRIVATE ${PLATFORM_WINDOWS_SOURCES})
elseif(CMAKE_SYSNAME STREQUAL "Linux")
	target_sources(rtengine PRIVATE ${PLATFORM_POSIX_SOURCES})
elseif(CMAKE_SYSNAME STREQUAL "Darwin")
	target_sources(rtengine PRIVATE ${PLATFORM_POSIX_SOURCES})
endif()

# Libraries