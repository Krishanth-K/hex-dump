cmake_minimum_required(VERSION 3.16)

project(cli
    VERSION 0.1
    LANGUAGES CXX
)

# ---- Compiler settings ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Warnings (don’t be a coward) ----
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ---- Sources ----
set(SOURCES
    src/main.cpp
    src/cli.cpp
    src/aho_corasick.cpp
    src/mapped_file.cpp
)

# ---- Target ----
add_executable(cli ${SOURCES})

target_include_directories(cli
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# ---- Optimization (optional, sane default) ----
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

