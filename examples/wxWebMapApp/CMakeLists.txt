
#find_package(Boost REQUIRED COMPONENTS thread timer unit_test_framework serialization filesystem iostreams)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

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

target_include_directories(wxWebMapApp PUBLIC "../wxWebMApApp")
#target_include_directories(IconicWebMapTest PUBLIC ${Boost_INCLUDE_DIRS})
#target_include_directories(IconicWebMapTest PUBLIC ${EIGEN3_INCLUDE_DIRS})
#target_include_directories(IconicWebMapTest PUBLIC ${GLFW_INCLUDE_DIRS})
target_include_directories(wxWebMapApp PUBLIC "../../include")

target_link_libraries(wxWebMapApp 
# ${INSTALL_BIN_DIR}
    ${Boost_LIBRARIES}
    ${wxWidgets_LIBRARIES}
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
