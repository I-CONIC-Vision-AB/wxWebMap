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

# Include functions
include(GenerateExportHeader)

# Define a variable containing a list of source files for the project
set(SRC_FILES
    wxMapHtml.cpp
    wxMapImage.cpp
    wxMapMarker.cpp
    wxMapObject.cpp
    wxMapPolygon.cpp
    wxWebMap.cpp
)

if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
    list(APPEND SRC_FILES ../../src/osx/carbon/wxmac.icns)
endif()

# Define the build target for the executable
add_library(IconicWebMap SHARED)
target_sources(IconicWebMap PRIVATE ${SRC_FILES})

generate_export_header(IconicWebMap
    BASE_NAME iconic_webmap
    EXPORT_FILE_NAME "${PROJECT_SOURCE_DIR}/exports/webmap_exports.h"
)

target_compile_definitions(IconicWebMap PRIVATE 
    ${wxWidgets_DEFINITIONS}
    "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>"
)

target_include_directories(IconicWebMap
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/exports}>
		$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	PRIVATE
        ${wxWidgets_INCLUDE_DIRS}
)

target_include_directories(IconicWebMap PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/exports>
    $<INSTALL_INTERFACE:exports>
)

# Include directories
if(WIN32)
    # TODO: Will only work for Windows, what about Linux?
    # https://stackoverflow.com/questions/25676277/cmake-target-include-directories-prints-an-error-when-i-try-to-add-the-source
    target_include_directories(IconicWebMap PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/out/build/windows-x64-release/wxWidgets/lib/vc_x64_dll/mswu>
        $<INSTALL_INTERFACE:out/build/windows-x64-release/wxWidgets/lib/vc_x64_dll/mswu>
    )
else()
    # Might be needed when building wxWidgets on Linux. Not sure if it has effect - remove if it doesn't help.
    target_compile_options(IconicWebMap PRIVATE -fPIC -fno-jump-tables -Wl,--hash-style=sysv)
    # We build wxWidgets ourselves when running on Linux, so make sure to add include directories
    target_include_directories(IconicWebMap PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/wxWidgets/include>
        $<INSTALL_INTERFACE:wxWidgets/include>
    )
endif()

# Linking
# See this for how to link with wxWidgets: https://docs.wxwidgets.org/3.2.0/overview_cmake.html
if(WIN32)
    # For Windows, we link with the vcpkg-version of wxWidgets. This is how to link with the libraries
    target_link_libraries(IconicWebMap ${wxWidgets_LIBRARIES})
else()
    # When we include wxWidgets as a subdirectory, this is how to link with the components
    target_link_libraries(IconicWebMap wx::core wx::xml wx::webview wx::stc)
endif()

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "../../src/osx/carbon/wxmac.icns"
        MACOSX_BUNDLE_ICON_FILE wxmac.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright wxWidgets"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.wxwidgets.webview"
        )
endif()

if (VCPKG_TARGET_IS_WINDOWS)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/WebView2Loader.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/WebView2Loader.dll COPYONLY)
endif()

