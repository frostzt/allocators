cmake_minimum_required(VERSION 3.16)
project(allocators LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
  -Wall -Wextra -Werror -Wpedantic
  -O0 -g
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# Lib target
add_library(allocators STATIC
  allocators/dumbfuck/fixed_chunks/fixed_chunks.c
  allocators/pool/pool.c
)

target_include_directories(allocators PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# testing
enable_testing()

# test exec
add_executable(run_tests
  tests/main.c
  tests/dumbfuck/fixed_chunks/fixed_chunks_test.c
)

target_link_libraries(run_tests PRIVATE allocators_core)
target_include_directories(run_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

add_test(NAME all_tests COMMAND run_tests)
