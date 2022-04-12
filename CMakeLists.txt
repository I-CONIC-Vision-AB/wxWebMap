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
cmake_minimum_required(VERSION 2.8.12)

# Name the project
project(iconic-webmap)

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


# Request the required wxWidgets libs
find_path(BOOST_SHARED_PTR_INCLUDE_DIRS "boost/shared_ptr.hpp") # Make sure we have boost::shared_ptr for smart pointer handling
find_package(wxWidgets 3.1 COMPONENTS net core base html xml webview REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})
add_compile_definitions(WXUSINGDLL)

set(CMAKE_CXX_STANDARD 11)

include(GenerateExportHeader)
set(ICONIC_EXPORTS_DIR "${CMAKE_CURRENT_LIST_DIR}/exports")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy the template map.html file
#message(STATUS "in dir: ${CMAKE_SOURCE_DIR}/data/map.html")
#message(STATUS "out dir:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/map.html")
configure_file(${CMAKE_SOURCE_DIR}/data/html/map.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/html/map.html COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/iconic.ico ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iconic.ico COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/js/wxMapMarker.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapMarker.js COPYONLY)

add_subdirectory("src")
#add_subdirectory("webview")
add_subdirectory("examples")
add_subdirectory("test")