cmake_minimum_required(VERSION 3.10)
project(wsdl2c-native VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

# Include directories
include_directories(
    include
    ${LIBXML2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/axiom/include
    ${CMAKE_SOURCE_DIR}/util/include
)

# Source files
set(SOURCES
    src/main.c
    src/options.c
    src/wsdl_parser.c
    src/stub_generator.c
    # TODO: Add these when implemented
    # src/schema_parser.c
    # src/type_mapper.c
    # src/skel_generator.c
    # src/template_engine.c
)

# Create executable
add_executable(wsdl2c-native ${SOURCES})

# Link libraries
target_link_libraries(wsdl2c-native
    ${LIBXML2_LIBRARIES}
    # axutil
    # axiom
)

# Compiler flags
target_compile_options(wsdl2c-native PRIVATE
    ${LIBXML2_CFLAGS_OTHER}
    -Wall -Wextra -pedantic
    -DHAVE_CONFIG_H
)

# For standalone testing without full Axis2/C build
target_compile_definitions(wsdl2c-native PRIVATE
    AXIS2_SUCCESS=0
    AXIS2_FAILURE=-1
    AXIS2_TRUE=1
    AXIS2_FALSE=0
    AXIS2_LOG_LEVEL_INFO=2
    STANDALONE_BUILD=1
)

# Install target
install(TARGETS wsdl2c-native
    RUNTIME DESTINATION bin/tools/wsdl2c
)

# Install templates
install(DIRECTORY templates/
    DESTINATION share/axis2c/codegen/templates
    OPTIONAL
)