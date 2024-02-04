cmake_minimum_required(VERSION 3.22)

################################################################################
# Includes
################################################################################
include(FetchContent)
include(GoogleTest)

################################################################################
# Libs
################################################################################
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    URL_HASH SHA1=0AC421F2EC11AF38B0FFF0F1992184032731A8BC
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

################################################################################
# Source files
################################################################################
set(SRC_TESTS
    ut_boundedvalue.cpp
)

set(SRC_TEST_UTILS
    signalspy.hpp
)

################################################################################
# Targets setup
################################################################################
add_executable(ut_vol2com
    ${SRC_TESTS}
    ${SRC_TEST_UTILS}
)

target_link_libraries(ut_vol2com PRIVATE
    Qt::Core
    Qt::Gui
    GTest::gtest_main
    vol2com_lib
)

target_include_directories(ut_vol2com
    PUBLIC <$TARGET_PROPERTY:vol2com_lib,INCLUDE_DIRECTORIES>
           ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(ut_vol2com
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/unittest"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/unittest"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/unittest"
)

################################################################################
# Copy depended DLL's and other stuff to be able to build target ALL and run GTEST executable
################################################################################
if (WIN32)
   add_custom_command(TARGET ut_vol2com POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:appvol2com> $<TARGET_FILE_DIR:ut_vol2com>
       COMMAND_EXPAND_LISTS
   )
endif()

################################################################################
# GoogleTest Section
################################################################################
gtest_discover_tests(ut_vol2com DISCOVERY_TIMEOUT 10) # Set timeout to 10 minutes instead 5 as default
