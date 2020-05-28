# ---------------------------------------------------------------------------
# Frontend2 Author: jongough aka. Jon Gough    License:wxWidgets License
# ---------------------------------------------------------------------------
##---------------------------------------------------------------------------
## Author:      Jon Gough
##***************************************************************************
## *   Copyright (C) 2010 by Jon Gough                          *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,                                       *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx          *
## ***
#-----------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# ---------------------------------------------------------------------------
# CMake-CI Number: 0.0.007  
#    - 2/19/2020 10 environments needed for beta testing.
#    - 2/19/2020 fix debian build
#    - 2/19/2020 Add raspian build
#    - 2/20/2020 fix execute ci/*.sh files  and increment version patch
#    - 3/1/2020  fix raspian xml target-url with if-then
#    - 3/30/2020 MacOS build fixed 12 env
#    - Reverse dates here
#    - 2/05/2020 MacOS final fix- JG
#    - 15/05/2020 Additional Improvements  
#    - 26/05/2020 Add ARCH testing to pluginSetup.cmake
#    - 28/05/2020 Fix Local build 
#    - 28/05/2020 Work on Local Build
#    - 28/05/2020 Move CmakeLists code into PluginSetup.cmake 
# ---------------------------------------------------------------------------
##----- Modify section below to include all the details for your plugin ----- ##

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
  if(POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
  endif(POLICY CMP0043)
  cmake_policy(SET CMP0048 NEW)
  if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
  endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

project(testplugin_pi)

set(VERBOSE_NAME "Testplugin")
# Search "getcommonname"in weather_routing_pi.cpp
set(COMMON_NAME "testplugin")
set(TITLE_NAME "testplugin")
set(PACKAGE_CONTACT "Jon Gough")
set(PACKAGE "testplugin")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "100")
set(VERSION_TWEAK "0")
set(VERSION_DATE "28/05/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")

#set(GIT_USER "mauroc")
#set(GIT_USER "jongough")
#set(GIT_USER "rgleason")
#set(GIT_REPOSITORY_NAME "testplugin_pi")
# For setting up your own local webserver with git loaded, for testing purposes.
set(GIT_REPOSITORY_SERVER "github.com")

# Set Cloudsmith upload repository suffix for each catalog
set(PROD "prod")    #Standard Repos
set(BETA "beta")    #Standard Repos
set(ALPHA "alpha")  #Standard Repos

#set(CLOUDSMITH_USER "jon-gough")
#set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

# set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}") - This is the default)
# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
# set(CLOUDSMITH_BASE_REPOSITORY "squiddio")  # Mauro's customization without the '_pi'
# set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins") #Rick's customization
set (CLOUDSMITH_BASE_REPOSITORY "testplugin")  #without the pi 

set(XML_INFO_URL "https://opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:opencpn_user_manual:plugins:safety:odraw1.6_pi")
set(XML_SUMMARY "Plugin to test examples of the ODAPI and JSON interface for ODRAW 1234#")
set(XML_DESCRIPTION "Testplugin Plugin is used to test out the ODraw API and demonstrate how to use it successfully from another plugin")

##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

#------Settings
option(PLUGIN_USE_SVG "Use SVG graphics" ON)
set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")

# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" OFF)

message(STATUS "USE_GL: ${USE_GL}")
#------End Settings

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statement below is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
#===========================================================
INCLUDE("cmake/PluginSetup.cmake")
#===========================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

# Settings and Options
set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements ----- ##

add_definitions(-DUSE_S57)

# ------These lines below are only for Testplugin - do not use ------------##
# ----Remember to Add your project Set(SRCS and set(HDRS  etc..
set(SRCS
    src/tpdc.cpp
    src/tpControlDialogDef.cpp
    src/tpControlDialogImpl.cpp
    src/tpicons.cpp
    src/tpJSON.cpp
    src/testplugin_pi.cpp
    src/tpUtils.cpp)

set(HDRS
    include/testplugin_pi.h
    include/tpicons.h
    include/tpControlDialogDef.h
    include/tpControlDialogImpl.h
    include/tpIconCombo.h
    include/tpJSON.h
    include/tpdc.h
    include/tpUtils.h)

set(OCPNSRC ocpnsrc/TexFont.cpp)
set(OCPNHDRS ocpninclude/ocpn_plugin.h ocpninclude/pluginmanager.h ocpninclude/TexFont.h)

set(EXTSRC)

set(EXTINCLUDE
    extinclude/nlohmann/json.hpp
    extinclude/ODAPI.h)
set(LIBSSRC libs/wxJSON/src/jsonreader.cpp libs/wxJSON/src/jsonval.cpp libs/wxJSON/src/jsonwriter.cpp)

set(LIBHDRS
    libs/GL/include/gl.h
    libs/GL/include/gl_private.h
    libs/GL/include/glext.h
    libs/GL/include/glu.h
    libs/wx/include/json_defs.h
    libs/wx/include/jsonreader.h
    libs/wx/include/jsonval.h
    libs/wx/include/jsonwriter.h)

add_definitions(-DPLUGIN_USE_SVG)

IF(APPLE)
    OPTION(TP_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
ELSE(APPLE)
    OPTION(TP_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
ENDIF(APPLE)

IF(TP_JSON_SCHEMA_VALIDATOR)
    MESSAGE(STATUS "Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
    ADD_DEFINITIONS(-DTP_JSON_SCHEMA_VALIDATOR)
    MESSAGE(STATUS "Adding JSON Schema validation")

    SET(EXTSRC
        ${EXTSRC}
        extsrc/json-schema-validator/json-schema-draft7.json.cpp
        extsrc/json-schema-validator/json-uri.cpp
        extsrc/json-schema-validator/json-validator.cpp
    )
    SET(EXTINCLUDE
        ${EXTINCLUDE}
        extinclude/nlohmann/json.hpp
        extinclude/json-schema-validator/json-schema.hpp
    )
    INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/extinclude/json-schema-validator)
ELSE(TP_JSON_SCHEMA_VALIDATOR)
    MESSAGE(STATUS "Not using JSON Schema validation - warning invalid JSON 'may' cause unexpected crashes")
ENDIF(TP_JSON_SCHEMA_VALIDATOR)
##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##================================================================================	
#    SET(EXTSRC  ${EXTSRC}   etc   Can be in an if statement as below
##================================================================================
add_library(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSSRC})

add_definitions(-DTIXML_USE_STL)

##================================================================================
#JSON Validator: Every plugin that uses JSON for passing messages should use a validator.  A JSON validator is much like an XML validator, the validator allows the system to ignore non-conformant messages.  The JSON validator changes the OSABI flavor of the Linux executable so there is code to change it back.

IF(UNIX AND NOT APPLE)
  IF(TP_JSON_SCHEMA_VALIDATOR)
    # Change OS/ABI to be UNIX - System V to make it run with OCPN
    SET(lib_name lib${PACKAGE_NAME}.so)
    MESSAGE(STATUS "lib-name: ${lib_name}")
    FIND_FILE(elfedit elfedit)
    IF(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
      MESSAGE(STATUS "Will ensure OS/ABI is set to 0 to match OCPN")
      ADD_CUSTOM_COMMAND(
        TARGET ${PACKAGE_NAME}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${PACKAGE_NAME}
        COMMENT " Running post build action on ${lib_name}."
        COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) '
      )
    ELSE(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
      MESSAGE(STATUS "Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
    ENDIF(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
  ENDIF(TP_JSON_SCHEMA_VALIDATOR)
ENDIF(UNIX AND NOT APPLE)
##================================================================================
## ----- Change above to match project requirements ----- ##

#These are NOT used in Testplugin, used for other plugins
#INCLUDE("cmake/PluginJSON.cmake")
#INCLUDE("cmake/PluginXML.cmake")
#INCLUDE("cmake/PluginGL.cmake")

## ----- do not change next section - needed to configure build process ----- ##
include("cmake/PluginInstall.cmake")
include("cmake/PluginLocalization.cmake")
include("cmake/PluginPackage.cmake")
## ----- do not change section above - needed to configure build process ----- ##
