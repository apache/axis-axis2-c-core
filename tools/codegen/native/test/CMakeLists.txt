# ADB Test Suite for Axis2/C Native Generator
cmake_minimum_required(VERSION 3.10)

project(adb-tests C)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

# Test framework executable
add_executable(adb_test_runner
    src/adb_test_framework.c
    src/test_basic_types.c
    src/test_complex_types.c
    src/test_enumerations.c
    src/test_choices.c
    src/test_arrays.c
)

# Include directories
target_include_directories(adb_test_runner PRIVATE
    include
    ${LIBXML2_INCLUDE_DIRS}
    ../include
)

# Link libraries
target_link_libraries(adb_test_runner
    ${LIBXML2_LIBRARIES}
)

# Compiler flags
target_compile_options(adb_test_runner PRIVATE
    ${LIBXML2_CFLAGS_OTHER}
    -Wall -Wextra -std=c99
)

# Add feature test macros for POSIX functions
target_compile_definitions(adb_test_runner PRIVATE
    _GNU_SOURCE
    _POSIX_C_SOURCE=200809L
)

# Test WSDLs directory
set(TEST_WSDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wsdl)
target_compile_definitions(adb_test_runner PRIVATE
    TEST_WSDL_DIR="${TEST_WSDL_DIR}"
)

# Custom target to run all ADB tests
add_custom_target(run_adb_tests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/adb_test_runner
    DEPENDS adb_test_runner
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running ADB test suite"
)

# Integration test that uses the native generator
add_custom_target(integration_adb_test
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/integration/run_integration_tests.sh
    DEPENDS ../build/wsdl2c-native
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running ADB integration tests with native generator"
)