cmake_minimum_required (VERSION 3.20.0)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RTGame")

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# If we're not on VS, then copy compile_commands.json for LSPs like cla ngd and ccls.
message(":: Generator: " ${CMAKE_GENERATOR})
add_custom_command(
      OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json
              ${CMAKE_SOURCE_DIR}/compile_commands.json
      DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
      COMMENT "Copying compile_commands.json...")

    add_custom_target(copy_compile_commands ALL
                      DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)

# Include sub-projects.
add_subdirectory ("RTEngine")
add_subdirectory ("Game")
