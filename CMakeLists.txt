##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.1.1)
IF (COMMAND cmake_policy)
  if (POLICY CMP0043)
    CMAKE_POLICY(SET CMP0043 NEW)
  endif (POLICY CMP0043)
  CMAKE_POLICY(SET CMP0048 NEW)
  if (POLICY CMP0077)
    CMAKE_POLICY(SET CMP0077 NEW)
  endif (POLICY CMP0077)
ENDIF (COMMAND cmake_policy)

SET(PARENT "opencpn")
SET(PACKAGE "testplugin")
SET(VERBOSE_NAME "Testplugin")
SET(CPACK_PACKAGE_CONTACT "Jon Gough")
SET(CLOUDSMITH_USER "jongough")
SET(GIT_USER "jongough")
SET(GIT_REPOSITORY_NAME "testplugin_pi")

PROJECT(testplugin_pi)

SET(PACKAGE_NAME testplugin_pi)
SET(VERBOSE_NAME testplugin)
SET(TITLE_NAME testplugin)
SET(CPACK_PACKAGE_CONTACT "Jon Gough")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "32")
SET(VERSION_TWEAK "0")
SET(VERSION_DATE "05/11/2019")
SET(OCPN_MIN_VERSION "ov50")
SET(OCPN_API_VERSION_MAJOR "1")
SET(OCPN_API_VERSION_MINOR "16")
SET(TP_COMMENT "  * Release for O5 using CI")

SET(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
SET(OpenGL_GL_PREFERENCE "LEGACY")

SET(PACKAGE_NAME ${PACKAGE}_pi)
SET(PACKAGE_FILE_NAME "${PARENT}-plugin-${PACKAGE}")
STRING(TOUPPER "${PACKAGE}" TITLE_NAME)

PROJECT(${PACKAGE_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})

SET(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

MESSAGE(STATUS "${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")

# Don't use local version of GLU library
SET(USE_LOCAL_GLU FALSE)

IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build")
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel" )
MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION}-${OCPN_MIN_VERSION}-${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR})
SET(PKG_BASE_URL "https://dl.cloudsmith.io/public/${CLOUDSMITH_USER}/${PACKAGE}-pi/raw/files")

INCLUDE("cmake/PluginSetup.cmake")
set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

SET(PKG_RELEASE "1")
SET(PKG_NVR ${PACKAGE_NAME}-${PACKAGE_VERSION}.${PKG_RELEASE})
SET(PKG_BASE_URL "https://dl.cloudsmith.io/public/mauro-calvi/squiddio-pi/raw/files")

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
    ocpnsrc/TexFont.cpp
)
SET(OCPNHDRS
    ocpninclude/ocpn_plugin.h
    ocpninclude/pluginmanager.h
    ocpninclude/TexFont.h
)

SET(EXTSRC 
)

SET(EXTINCLUDE 
    extinclude/ODAPI.h
)

SET(LIBSSRC
    libs/wxJSON/src/jsonreader.cpp
    libs/wxJSON/src/jsonval.cpp
    libs/wxJSON/src/jsonwriter.cpp
)

SET(LIBHDRS
    libs/GL/include/gl.h
    libs/GL/include/gl_private.h
    libs/GL/include/glext.h
    libs/GL/include/glu.h
    libs/wx/include/json_defs.h
    libs/wx/include/jsonreader.h
    libs/wx/include/jsonval.h
    libs/wx/include/jsonwriter.h
)
OPTION(TESTPLUGIN_USE_SVG "Use SVG graphics" ON)
ADD_DEFINITIONS(-DTESTPLUGIN_USE_SVG)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSRC})

ADD_DEFINITIONS(-DTIXML_USE_STL)

INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/libs/GL/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

IF(NOT OCPN_FLATPAK)
    INCLUDE("cmake/PluginInstall.cmake")
    INCLUDE("cmake/PluginLocalization.cmake")
ENDIF(NOT OCPN_FLATPAK)
INCLUDE("cmake/PluginPackage.cmake")
