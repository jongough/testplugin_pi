##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
IF (COMMAND cmake_policy)
    if (POLICY CMP0043)
        CMAKE_POLICY(SET CMP0043 NEW)
    endif (POLICY CMP0043)
ENDIF (COMMAND cmake_policy)

PROJECT(testplugin_pi)

SET(PACKAGE_NAME testplugin_pi)
SET(VERBOSE_NAME testplugin)
SET(TITLE_NAME testplugin)
SET(CPACK_PACKAGE_CONTACT "Jon Gough")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "13")
SET(VERSION_DATE "09/12/2018")

SET(BUNDLE_DATA FALSE)
SET( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" )
SET_PROPERTY( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS None Debug Release RelWithDebInfo MinSizeRel )

MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(CMAKE_VERBOSE_MAKEFILE ON)

OPTION(OD_CXX11 "Use c++11" OFF)

IF ( OD_CXX11 )
    MESSAGE(STATUS "Attempting to use c++11")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        SET(Testplugin_USE_CXX11 ON)
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        SET(Testplugin_USE_CXX11 ON)
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
        SET(Testplugin_USE_CXX11 OFF)
    endif()
ELSE()
    SET(Testplugin_USE_CXX11 OFF)
ENDIF()

IF(Testplugin_USE_CXX11)
    MESSAGE(STATUS "Allowing use of c++11")
    ADD_DEFINITIONS(-DTestplugin_USE_CXX11)
ENDIF()

MESSAGE(STATUS "CMAKE version: ${CMAKE_VERSION}")
if ( CMAKE_VERSION VERSION_GREATER 3.4 )
    set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Add clang-tidy automatically to builds")
    if (ENABLE_CLANG_TIDY)
        find_program (CLANG_TIDY_EXE NAMES "clang-tidy" PATHS /usr/local/opt/llvm/bin )
        if (CLANG_TIDY_EXE)
            message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
            #For more, see http://clang.llvm.org/extra/clang-tidy/
            #set(CLANG_TIDY_CHECKS "-*,modernize-*")
            set(CLANG_TIDY_CHECKS "-*,performance-*")
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/*'"
                CACHE STRING "" FORCE)
        else()
            message(AUTHOR_WARNING "clang-tidy not found!")
            set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # delete it
        endif()
    endif()
endif()

if ( CMAKE_VERSION VERSION_GREATER 3.9 )
    set(ENABLE_CPPCHECK OFF CACHE BOOL "Add cppcheck automatically to builds")
    if (ENABLE_CPPCHECK)
        find_program (CPPCHECK_EXECUTABLE NAMES "cppcheck" )
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
    endif()
endif()

#SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/include/version.h)

INCLUDE("cmake/PluginConfigure.cmake")

ADD_DEFINITIONS(-DUSE_S57)


SET(SRCS
    src/tpdc.cpp
    src/tpControlDialogDef.cpp
    src/tpControlDialogImpl.cpp
    src/tpicons.cpp
    src/tpJSON.cpp
    src/testplugin_pi.cpp
    src/tpUtils.cpp
)

SET(HDRS
    include/testplugin_pi.h
    include/tpicons.h
    include/tpControlDialogDef.h
    include/tpControlDialogImpl.h
    include/tpIconCombo.h
    include/tpJSON.h
    include/tpdc.h
    include/tpUtils.h
)

SET(OCPNSRC 
    ocpnsrc/wxJSON/jsonreader.cpp
    ocpnsrc/wxJSON/jsonval.cpp
    ocpnsrc/wxJSON/jsonwriter.cpp
    ocpnsrc/TexFont.cpp
)
SET(OCPNHDRS
    ocpninclude/ocpn_plugin.h
    ocpninclude/pluginmanager.h
    ocpninclude/TexFont.h
    src/nmea0183/LatLong.hpp
    src/nmea0183/nmea0183.h
    src/nmea0183/nmea0183.hpp
    src/nmea0183/Response.hpp
    src/nmea0183/Sentence.hpp
    ocpninclude/wx/json_defs.h
    ocpninclude/wx/jsonreader.h
    ocpninclude/wx/jsonval.h
    ocpninclude/wx/jsonwriter.h
)

SET(EXTSRC 
)

SET(EXTINCLUDE 
    extinclude/ODAPI.h
)

OPTION(TESTPLUGIN_USE_SVG "Use SVG graphics" ON)
ADD_DEFINITIONS(-DTESTPLUGIN_USE_SVG)

IF(UNIX)
    INCLUDE("cmake/FindTinyXML.cmake")
    FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX)

IF(TINYXML_FOUND)
    message (STATUS "Building with system tinyxml")
    INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
    ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC})
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
ELSE(TINYXML_FOUND)
    message (STATUS "Building with embedded tinyxml")
    INCLUDE_DIRECTORIES(src/tinyxml/)
    SET(SRC_LTINYXML
        src/tinyxml/tinyxml.cpp
        src/tinyxml/tinyxmlparser.cpp
        src/tinyxml/tinyxmlerror.cpp
    )
    ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)

ADD_DEFINITIONS(-DTIXML_USE_STL)

INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/src/mygdal  )
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/src/nmea0183)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
