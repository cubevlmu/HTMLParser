cmake_minimum_required(VERSION 3.20)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmakes/index.cmake)
#################################################################
project(HTMLParser)
#################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJC_STANDARD 17)
set(CMAKE_OBJCXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-deprecated-literal-operator)
endif()
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions")
#################################################################
set(INCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
set(LNKS
        ""
)
set(DEPS
        ""
)
set(HEAD
        ""
)
set(DEFS
        "JT_DEBUG"
)
set(PJNAME "HTMLParser")
#################################################################
# dependencies
#################################################################
SetPlatformDeps(INCS LNKS)
#################################################################
# project configurations
#################################################################
file(GLOB_RECURSE src
        "src/*.m"
        "src/*.cpp"
        "src/*.mm"
        "src/*.c"
)
add_library(${PJNAME} STATIC
        ${src}
)
target_include_directories(${PJNAME} PRIVATE
        ${INCS}
)
target_link_libraries(${PJNAME} PRIVATE
        ${LNKS}
)
target_compile_definitions(${PJNAME} PRIVATE
        ${DEFS}
)
#################################################################
include(GNUInstallDirs)
install(TARGETS ${PJNAME}
        EXPORT HTMLParserTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/html/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/HTMLParser
        FILES_MATCHING PATTERN "*.hpp"
)
install(EXPORT HTMLParserTargets
        FILE HTMLParserConfig.cmake
        NAMESPACE HTMLParser::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HTMLParser
)