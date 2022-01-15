
#find_package(Boost REQUIRED COMPONENTS thread timer unit_test_framework serialization filesystem iostreams)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(SOURCES
    "IconicWebMapTest.cpp"
)

add_executable(IconicWebMapTest ${SOURCES})

target_include_directories(IconicWebMapTest PUBLIC "../test" "../include")
target_compile_definitions(IconicWebMapTest PUBLIC BOOST_TEST_DYN_LINK)

target_link_libraries(IconicWebMapTest 
# ${INSTALL_BIN_DIR}
    ${Boost_LIBRARIES}
    ${wxWidgets_LIBRARIES}
    IconicWebMap
)

install(TARGETS IconicWebMapTest  
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
