set(SOURCES
    "wxWebMapApp.cpp"
    "wxWebMapFrame.cpp"
    "SourceViewDialog.cpp"
    "PolygonReader.cpp"
    "ImageReader.cpp"
)

if(WIN32)
    # Include a RC file for windows
    list(APPEND SOURCES ../sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
#    list(APPEND SRC_FILES ../../src/osx/carbon/wxmac.icns)
endif()

# Define the build target for the executable
add_executable(wxWebMapApp WIN32 MACOSX_BUNDLE ${SOURCES})

target_include_directories(wxWebMapApp PUBLIC ${wxWidgets_INCLUDE_DIRS})

# Include files from the current (wxWebMapApp) directory
target_include_directories(wxWebMapApp PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:.>
)

# Include files from the root (wxWebMap) directory
target_include_directories(wxWebMapApp PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:../../include>
)

# Link required libraries to the executable
target_link_libraries(wxWebMapApp 
	wx::base
	wx::core
	wx::webview
	wx::xml
	wx::html
	wx::stc
    IconicWebMap
)

install(TARGETS wxWebMapApp  
    EXPORT iconicTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
)

install(
    DIRECTORY 
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" 
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    FILES_MATCHING PATTERN "*.exe"
)
