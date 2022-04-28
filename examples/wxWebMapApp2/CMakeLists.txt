
find_package(wxWidgets 3.1 COMPONENTS stc REQUIRED)

set(SOURCES
    "wxWebMapApp.cpp"
    "wxWebMapFrame.cpp"
    "SourceViewDialog.cpp"
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

target_compile_definitions(wxWebMapApp PRIVATE ${wxWidgets_DEFINITIONS} "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>")

target_include_directories(wxWebMapApp PUBLIC "../wxWebMapApp" ${wxWidgets_INCLUDE_DIRS})
#target_include_directories(IconicWebMapTest PUBLIC ${Boost_INCLUDE_DIRS})
#target_include_directories(IconicWebMapTest PUBLIC $ {EIGEN3_INCLUDE_DIRS})
#target_include_directories(IconicWebMapTest PUBLIC ${GLFW_INCLUDE_DIRS})
target_include_directories(wxWebMapApp PUBLIC "../../include")

target_link_libraries(wxWebMapApp 
# ${INSTALL_BIN_DIR}
    ${Boost_LIBRARIES}
    ${wxWidgets_LIBRARIES}
    IconicWebMap
)

if(USE_WXRC)
    execute_process(
        COMMAND "${wxWidgets_wxrc_EXECUTABLE}" --help
        RESULTS_VARIABLE error_result
    )
    if(error_result)
        message(FATAL_ERROR "Failed to run wxWidgets_wxrc_EXECUTABLE (${wxWidgets_wxrc_EXECUTABLE})")
    endif()
endif()

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
