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
    EXPORT_FILE_NAME "${ICONIC_WEBMAP_EXPORTS_DIR}/webmap_exports.h"
)

target_compile_definitions(IconicWebMap PRIVATE ${wxWidgets_DEFINITIONS} "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>")

target_include_directories(IconicWebMap
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${ICONIC_WEBMAP_EXPORTS_DIR}>
		$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	PRIVATE
        ${wxWidgets_INCLUDE_DIRS}
)

# Link required libraries to the executable
# For wxWidget libraries, see options in root CMakeList file. There are multiple options
target_link_libraries(IconicWebMap ${wxWidgets_LIBRARIES})

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

