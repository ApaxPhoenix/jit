cmake_minimum_required(VERSION 3.20)
project(polytoria CXX)

set(CMAKE_CXX_STANDARD 23)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/vm/src")
    message(STATUS "vm not found, cloning...")
    execute_process(
            COMMAND git clone https://github.com/ApaxPhoenix/vm.git
            ${CMAKE_SOURCE_DIR}/vendor/vm
            RESULT_VARIABLE GIT_RESULT
    )
    if(NOT GIT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to clone vm")
    endif()
endif()

add_subdirectory(vendor/vm)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(polytoria ${SOURCES} ${HEADERS})

target_compile_definitions(vm PRIVATE
        INIT_LUA_PATH="${CMAKE_SOURCE_DIR}/vendor/vm/init.lua"
)

target_include_directories(polytoria PRIVATE
        include
        vendor/vm/include
        vendor/vm/src
        vendor/vm/luajit_install/include/luajit-2.1
)

target_link_libraries(polytoria PRIVATE vm)