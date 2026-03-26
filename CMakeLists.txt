cmake_minimum_required(VERSION 3.20)
project(polytoria CXX)

set(CMAKE_CXX_STANDARD 17)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/vm/src/sandbox.c")
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

add_executable(polytoria ${SOURCES})

add_custom_command(TARGET polytoria POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/vendor/vm/scripts
        $<TARGET_FILE_DIR:polytoria>/scripts
)

target_include_directories(polytoria PRIVATE
        include
        vendor/vm/include
        "${CMAKE_BINARY_DIR}/vendor/vm/luajit_install/include/luajit-2.1"
)

target_link_libraries(polytoria PRIVATE vm)