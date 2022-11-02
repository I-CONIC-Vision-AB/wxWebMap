#############################################################################
# Name:        samples/minimal/CMakeListst.txt
# Purpose:     Sample CMake file to show usage of cmake for wxWidgets users
# Author:      Tobias Taschner
# Created:     2016-10-23
# Copyright:   (c) 2016 wxWidgets development team
# Licence:     wxWindows licence
#############################################################################

#
# This file is just a sample to show using cmake from an application
# If you want to build the minimal and other samples please use the
# wxBUILD_SAMPLES option when using cmake on the library
#

# Declare the minimum required CMake version
cmake_minimum_required(VERSION 3.22.1)

# Name the project
project(iconic-webmap)

# For wxWidgets usage, see https://docs.wxwidgets.org/3.2.0/overview_cmake.html
# Option 1 - using find_package
find_package(wxWidgets REQUIRED COMPONENTS net core base html xml webview stc)
include(${wxWidgets_USE_FILE})

SET(wxWidgets_USE_UNICODE OFF)

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

# Use C++17
set(CMAKE_CXX_STANDARD 17)

include(GenerateExportHeader)
set(ICONIC_WEBMAP_EXPORTS_DIR "${CMAKE_CURRENT_LIST_DIR}/exports")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy the template map.html file
configure_file(${PROJECT_SOURCE_DIR}/data/html/map.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/html/map.html COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/iconic.ico ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iconic.ico COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/js/wxMapMarker.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapMarker.js COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/js/wxMapPolygon.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapPolygon.js COPYONLY)
if (WIN32)
    # Needed for Windows Edge backend
    configure_file(${PROJECT_SOURCE_DIR}/cmake/WebView2Loader.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" COPYONLY)
endif(WIN32)

add_subdirectory("src")
add_subdirectory("examples")
# Tests are not completely implemented yet, so skip for now
#add_subdirectory("test")

### INSTALLATION ###

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/CMake")
set(INSTALL_DATA_DIR "${CMAKE_INSTALL_PREFIX}")


# Create a list of all targets
install(
    TARGETS IconicWebMap
    EXPORT IconicWebMapTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
)

install(
    EXPORT IconicWebMapTargets FILE "IconicWebMapTargets.cmake"
    EXPORT_LINK_INTERFACE_LIBRARIES
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

export(EXPORT IconicWebMapTargets 
    FILE "${CMAKE_CURRENT_BINARY_DIR}/IconicWebMapTargets.cmake"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "cmake/IconicWebMapConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/IconicWebMapConfig.cmake"
    INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}"
    PATH_VARS INSTALL_INCLUDE_DIR INSTALL_DATA_DIR
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/IconicWebMapConfig.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

install(
    DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include"
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
    FILES_MATCHING 
		PATTERN "*.h" 
        PATTERN "*.hpp"
)

install(
    FILES "${ICONIC_WEBMAP_EXPORTS_DIR}/webmap_exports.h"
    DESTINATION "${INSTALL_INCLUDE_DIR}"
)

# Copy DLL:s to target directory
if(WIN32)
	install(
		DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" 
		DESTINATION ${CMAKE_INSTALL_PREFIX}
		FILES_MATCHING PATTERN "*.dll"
	)
endif()

# Install data files
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/data" 
    DESTINATION "${INSTALL_DATA_DIR}"
    FILES_MATCHING 
		PATTERN "*.html" 
		PATTERN "*.js" 
		PATTERN "*.ico"
)

install(
    FILES "${CMAKE_SOURCE_DIR}/data/html/map.html"
    DESTINATION "${INSTALL_BIN_DIR}/html"
)

install(
    FILES "${CMAKE_SOURCE_DIR}/data/js/wxMapMarker.js"
    DESTINATION "${INSTALL_BIN_DIR}/js"
)

install(
    FILES "${CMAKE_SOURCE_DIR}/data/js/wxMapPolygon.js"
    DESTINATION "${INSTALL_BIN_DIR}/js"
)
