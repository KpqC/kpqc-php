cmake_minimum_required(VERSION 3.20)
project(kpqc_php VERSION 0.1.0 LANGUAGES C CXX)

option(KPQC_BUILD_NATIVE_TESTS "Build the bundled native API and KAT tests" OFF)

find_program(PHP_CONFIG_EXECUTABLE NAMES php-config REQUIRED)
get_filename_component(PHP_BINARY_HINT "${PHP_CONFIG_EXECUTABLE}" DIRECTORY)
find_program(PHP_EXECUTABLE NAMES php HINTS "${PHP_BINARY_HINT}" REQUIRED)

execute_process(
    COMMAND "${PHP_CONFIG_EXECUTABLE}" --includes
    OUTPUT_VARIABLE PHP_INCLUDE_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY)
separate_arguments(PHP_INCLUDE_FLAGS NATIVE_COMMAND "${PHP_INCLUDE_FLAGS}")
set(PHP_INCLUDE_DIRS)
foreach(flag IN LISTS PHP_INCLUDE_FLAGS)
    if(flag MATCHES "^-I(.+)$")
        list(APPEND PHP_INCLUDE_DIRS "${CMAKE_MATCH_1}")
    endif()
endforeach()
if(NOT PHP_INCLUDE_DIRS)
    message(FATAL_ERROR "php-config did not report PHP include directories")
endif()

execute_process(
    COMMAND "${PHP_CONFIG_EXECUTABLE}" --extension-dir
    OUTPUT_VARIABLE PHP_EXTENSION_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY)
if(NOT PHP_EXTENSION_DIR)
    message(FATAL_ERROR "php-config did not report the PHP extension directory")
endif()
set(KPQC_PHP_EXTENSION_DIR "${PHP_EXTENSION_DIR}" CACHE PATH
    "Directory in which to install kpqc.so")

set(KPQC_BUILD_TESTS "${KPQC_BUILD_NATIVE_TESTS}" CACHE BOOL "" FORCE)
set(KPQC_INSTALL OFF CACHE BOOL "" FORCE)
set(KPQC_VERSION_OVERRIDE "${PROJECT_VERSION}")
add_subdirectory(native/kpqc)

add_library(kpqc_extension MODULE ext/kpqc.cpp)
target_compile_features(kpqc_extension PRIVATE cxx_std_17)
target_include_directories(kpqc_extension SYSTEM PRIVATE ${PHP_INCLUDE_DIRS})
target_link_libraries(kpqc_extension PRIVATE kpqc::kpqc)
set_target_properties(kpqc_extension PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    OUTPUT_NAME kpqc
    PREFIX ""
    VISIBILITY_INLINES_HIDDEN YES)

if(NOT MSVC)
    target_compile_options(kpqc_extension PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
if(APPLE)
    target_link_options(kpqc_extension PRIVATE
        "LINKER:-undefined,dynamic_lookup")
endif()

install(TARGETS kpqc_extension
    LIBRARY DESTINATION "${KPQC_PHP_EXTENSION_DIR}"
    RUNTIME DESTINATION "${KPQC_PHP_EXTENSION_DIR}")

enable_testing()
add_test(
    NAME php-api
    COMMAND "${PHP_EXECUTABLE}"
            -d "extension=$<TARGET_FILE:kpqc_extension>"
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/api_test.php")

if(KPQC_BUILD_NATIVE_TESTS)
    add_library(kpqc_test_extension MODULE
        ext/kpqc.cpp
        native/kpqc/tests/kat_drbg.cpp)
    target_compile_features(kpqc_test_extension PRIVATE cxx_std_17)
    target_include_directories(kpqc_test_extension
        SYSTEM PRIVATE ${PHP_INCLUDE_DIRS})
    target_include_directories(kpqc_test_extension PRIVATE
        native/kpqc/tests)
    target_link_libraries(kpqc_test_extension PRIVATE kpqc_test_core)
    target_compile_definitions(kpqc_test_extension PRIVATE
        KPQC_TESTING=1)
    set_target_properties(kpqc_test_extension PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        OUTPUT_NAME kpqc_test
        PREFIX ""
        VISIBILITY_INLINES_HIDDEN YES)
    if(NOT MSVC)
        target_compile_options(kpqc_test_extension PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    if(APPLE)
        target_link_options(kpqc_test_extension PRIVATE
            "LINKER:-undefined,dynamic_lookup")
    endif()

    add_test(
        NAME php-kat
        COMMAND "${PHP_EXECUTABLE}"
                -d "extension=$<TARGET_FILE:kpqc_test_extension>"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/kat_test.php")
endif()
