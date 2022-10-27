cmake_minimum_required(VERSION 3.22.1)

project(iconic-webmap)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# --------------------------------------------------------------------------
# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
if(WIN32)
    if(NOT BORLAND)
        if(NOT CYGWIN)
            if(NOT MINGW)
                if(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
                    add_definitions(
                    -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
                    -D_CRT_IS_WCTYPE_NO_DEPRECATE
                    -D_CRT_MANAGED_FP_NO_DEPRECATE
                    -D_CRT_NONSTDC_NO_DEPRECATE
                    -D_CRT_SECURE_NO_DEPRECATE
                    -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
                    -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
                    -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
                    -D_CRT_VCCLRIT_NO_DEPRECATE
                    -D_SCL_SECURE_NO_DEPRECATE
                    )
                endif()
                add_definitions(-DNOMINMAX)
            endif()
        endif()
    endif()
add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

include(GenerateExportHeader)
set(ICONIC_WEBMAP_EXPORTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/exports")
set(ICONIC_WEBMAP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy the template map.html file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/html/map.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/html/map.html COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/iconic.ico ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iconic.ico COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/js/wxMapMarker.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapMarker.js COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/js/wxMapPolygon.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapPolygon.js COPYONLY)

# See https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
#find_package(Git QUIET)
# Update all submodules
#if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
#    message(STATUS "Submodule update")
#    message(STATUS ${GIT_EXECUTABLE})
#    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
#    message(STATUS "Submodule update DONE: ${GIT_SUBMOD_RESULT}")
#    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#    endif()
#endif()

#if(NOT EXISTS "${PROJECT_SOURCE_DIR}/wxwidgets/CMakeLists.txt")
#    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#endif()

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

set(wxWidgets_USE_UNICODE off)
add_subdirectory("wxWidgets")

add_subdirectory("src")

# Only build example projects if the build is started with wxWebMap as root.
# If wxWebMap is build as a subproject, it is not built
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    add_subdirectory("examples")
endif()

target_include_directories(IconicWebMap PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/wxWidgets/include>
    $<INSTALL_INTERFACE:wxWidgets/include>
)
target_include_directories(IconicWebMap PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/exports>
    $<INSTALL_INTERFACE:exports>
)

# TODO: Will only work for Windows, what about Linux?
# https://stackoverflow.com/questions/25676277/cmake-target-include-directories-prints-an-error-when-i-try-to-add-the-source
target_include_directories(IconicWebMap PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/out/build/windows-x64-release/wxWidgets/lib/vc_x64_dll/mswu>
    $<INSTALL_INTERFACE:out/build/windows-x64-release/wxWidgets/lib/vc_x64_dll/mswu>
)

# See this for how to link with wxWidgets: https://docs.wxwidgets.org/3.2.0/overview_cmake.html
target_link_libraries(IconicWebMap wx::core wx::xml wx::webview wx::stc)

# Set install paths
set(INSTALL_BIN_DIR     "${CMAKE_INSTALL_PREFIX}/bin")
set(INSTALL_LIB_DIR     "${CMAKE_INSTALL_PREFIX}/lib")
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(INSTALL_CMAKE_DIR   "${CMAKE_INSTALL_PREFIX}/CMake")

# Define rules for installing this targets and associates with target name IconicWebMapTargets. 
#   We want to use target IconicWebMap and also targets from wxWidgets that we rely on
# Set output paths for:
#   Runtime (executables and .dll)
#   Archive (static libraries and .lib)
#   Library (shared libraries, except from .dll)
install(
    TARGETS IconicWebMap wxcore wxbase wxwebview wxxml wxstc
    EXPORT IconicWebMapTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
)

# Generate and install CMake file for importing targets to in another project.
# EXPORT_LINK_INTERFACE_LIBRARIES makes it possible to use these targets with
# older CMake versions. see: https://cmake.org/cmake/help/latest/command/export.html
install(
    EXPORT IconicWebMapTargets
    FILE "IconicWebMapTargets.cmake"
    EXPORT_LINK_INTERFACE_LIBRARIES
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

# Export CMake file with targets to be used for other projects
export(
    EXPORT IconicWebMapTargets 
    FILE "${CMAKE_CURRENT_BINARY_DIR}/IconicWebMapTargets.cmake"
)

# Import utility functions for generating config files (IconicWebMapConfig.cmake / IconicWebMap-config.cmake)
# https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html
include(CMakePackageConfigHelpers)

# Parameters:
#   input_file
#   output_file
#   path
#   list of directories used during install
configure_package_config_file(
    "cmake/IconicWebMapConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/IconicWebMapConfig.cmake"
    INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}"
    PATH_VARS INSTALL_INCLUDE_DIR CMAKE_INSTALL_PREFIX
)

# Copy config file to install directory
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/IconicWebMapConfig.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

# Copy include files to install directory
install(
    DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include"
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
    FILES_MATCHING 
		PATTERN "*.h" 
        PATTERN "*.hpp"
)

# Copy export file to install directory
install(
    FILES "${ICONIC_WEBMAP_EXPORTS_DIR}/webmap_exports.h"
    DESTINATION "${INSTALL_INCLUDE_DIR}"
)

# Copy DLL:s to install directory
#if(WIN32)
	install(
		DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" 
		DESTINATION ${CMAKE_INSTALL_PREFIX}
		FILES_MATCHING
            PATTERN "*.dll"
            PATTERN "*.so"
	)
#endif()

# Copy data files to install directory
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data" 
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
    FILES_MATCHING 
		PATTERN "*.html" 
		PATTERN "*.js" 
		PATTERN "*.ico"
)

# Copy map.html to install directory
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/html/map.html"
    DESTINATION "${INSTALL_BIN_DIR}/html"
)

# Copy wxMapMarker.js to install directory
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/js/wxMapMarker.js"
    DESTINATION "${INSTALL_BIN_DIR}/js"
)

# Copy wxMapPolygon.js to install directory
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/js/wxMapPolygon.js"
    DESTINATION "${INSTALL_BIN_DIR}/js"
)

#dump_cmake_variables()