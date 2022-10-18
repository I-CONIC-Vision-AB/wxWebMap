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
set(ICONIC_WEBMAP_EXPORTS_DIR "${CMAKE_CURRENT_LIST_DIR}/exports")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy the template map.html file
configure_file(${CMAKE_SOURCE_DIR}/data/html/map.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/html/map.html COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/iconic.ico ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iconic.ico COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/js/wxMapMarker.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapMarker.js COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/data/js/wxMapPolygon.js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/js/wxMapPolygon.js COPYONLY)

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

#message(STATUS "Before add")
add_subdirectory("wxwidgets")

install(TARGETS wx_webview DESTINATION asd2) # FOrtsätt här

#find_package(wxWidgets PATHS C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib NO_DEFAULT_PATH)
#find_package(wxWidgets)# COMPONENTS webview REQUIRED)
#message(FATAL_ERROR "wxWidgets libraries: ${wxWidgets_LIBRARIES}" )

#Here, we want to get a list of .lib files. See how wxWidgets_LIBRARIES looks like if we use find_package connected to vcpkg:
#debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxbase31ud_net.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxbase31u_net.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxmsw31ud_core.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_core.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxbase31ud.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxbase31u.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxmsw31ud_aui.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_aui.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxmsw31ud_html.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_html.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxbase31ud_xml.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxbase31u_xml.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxmsw31ud_gl.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_gl.lib;debug;C:/dev/vcpkg/installed/x64-windows/debug/lib/wxmsw31ud_webview.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_webview.lib;opengl32;glu32;winmm;comctl32;uuid;oleacc;uxtheme;rpcrt4;shlwapi;version;wsock32
SET(wxWidgets_LIBRARIES "debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxmsw33ud_stc.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxbase33ud_net.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxbase31u_net.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxmsw33ud_core.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_core.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxbase33ud.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxbase31u.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxmsw33ud_aui.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_aui.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxmsw33ud_html.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_html.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxbase33ud_xml.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxbase31u_xml.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxmsw33ud_gl.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_gl.lib;debug;C:/Users/andre/source/repos/wxWebMap/build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/wxmsw33ud_webview.lib;optimized;C:/dev/vcpkg/installed/x64-windows/lib/wxmsw31u_webview.lib;opengl32;glu32;winmm;comctl32;uuid;oleacc;uxtheme;rpcrt4;shlwapi;version;wsock32")

#find_package(wxWidgets REQUIRED COMPONENTS net core base html xml webview stc)
#include(${wxWidgets_USE_FILE})
SET(wxWidgets_USE_UNICODE OFF)

add_subdirectory("src")
add_subdirectory("examples")

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

#target_include_directories(IconicWebMap PRIVATE "wxwidgets/include")
# TODO: Will only work for Windows, what about Linux?
#target_include_directories(IconicWebMap PRIVATE "build/windows-x64-debug/wxwidgets/lib/vc_x64_dll/mswud")