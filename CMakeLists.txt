cmake_minimum_required(VERSION 3.20)
## USER_SECTION_START 1

## USER_SECTION_END


## INFO
## Template version: 1.6.1
##
## This is a template for a CMake project.
## It is designed to be used with the Visual Studio
## IDE and the Qt framework.
##
## ********************************************************************************
##  !!! IMPORTANT !!!
##  CMAKE SETTINGS WITH THE MARK "<AUTO_REPLACED>" ARE AUTOMATICLY REPLACED
##  BY THE LIBRARY MANAGER. DO NOT CHANGE NAMES OF SUCH PARAMETERS IF YOU USE
##  THE LIBRARY MANAGER.
##
##
## ********************************************************************************
##
## You can add your own settings in between the USER SECTION START and USER SECTION END markers.
## Do not add more USER SECTION START or USER SECTION END markers. 
## Code outside of the USER SECTION START and USER SECTION END markers will be overwritten by the library manager.
## 
## --------------------------------------------------------------------------------
## -------------------- Project settings ------------------------------------------
## --------------------------------------------------------------------------------
include(cmake/utility.cmake)

# Name of the library
set(LIBRARY_NAME QSFML_EditorWidget)                   # <AUTO_REPLACED>

# Library version (used for CMake package version file)
set(LIBRARY_VERSION "1.0.0")                    # <AUTO_REPLACED>

# Define is only active if the library is built
set(LIB_DEFINE QSFML_EDITOR_WIDGET_LIB)                # <AUTO_REPLACED>

# macro to enable profiling
set(LIB_PROFILE_DEFINE QSFML_PROFILING)  # <LIB_PROFILE_DEFINE>  


# Enable/disable QT modules
set(QT_ENABLE ON)	                            # <AUTO_REPLACED> 
# Enable/disable QT deployment
set_if_not_defined(QT_DEPLOY ON)	            # <AUTO_REPLACED> 

# Needed QT modules                             # <AUTO_REPLACED> 
set(QT_MODULES
  Core
  Widgets
)

# Library file name settings
set(DEBUG_POSTFIX_STR -d)                     # <AUTO_REPLACED> 
set(STATIC_POSTFIX_STR -s)                    # <AUTO_REPLACED> 
set(PROFILING_POSTFIX_STR -p)                 # <AUTO_REPLACED>  

# Langauge settings
set(CMAKE_CXX_STANDARD 17)                      # <AUTO_REPLACED> 
set(CMAKE_CXX_STANDARD_REQUIRED ON)             # <AUTO_REPLACED> 

## USER_SECTION_START 2

## USER_SECTION_END

# --------------------------------------------------------------------------------
# -------------------- Build settings --------------------------------------------
# --------------------------------------------------------------------------------

set_if_not_defined(INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/installation)   # Default install path
set_if_not_defined(RELATIVE_BUILD_FOLDER build)                       # Relative path to the build folder
set_if_not_defined(RELATIVE_INSTALL_LIB_FOLDER lib)                   # Relative path to the install folder
set_if_not_defined(RELATIVE_INSTALL_BIN_FOLDER bin)                   # Relative path to the install binary folder
set_if_not_defined(RELATIVE_INSTALL_INCLUDE_FOLDER include)           # Relative path to the install include folder


set_if_not_defined(COMPILE_EXAMPLES ON)						# <AUTO_REPLACED> 
set_if_not_defined(COMPILE_UNITTESTS ON)					# <AUTO_REPLACED> 


# User specific defines which are added as defines to the compiler
# Note: The defines are only available inside the library.
#       Do not use them in headers which are visible by the user of the library.
# Example:
# list(APPEND USER_SPECIFIC_DEFINES 
#    TEST_DEFINE
# )
# 
list(APPEND USER_SPECIFIC_DEFINES)

# User specific defines which are added as defines to the compiler
# Note: The defines are available for all dependencies and the library.
# Example:
# list(APPEND USER_SPECIFIC_GLOBAL_DEFINES 
#    TEST_DEFINE
# )
# 
list(APPEND USER_SPECIFIC_GLOBAL_DEFINES)


## USER_SECTION_START 3

## USER_SECTION_END

# --------------------------------------------------------------------------------
# -------------------- Dependencies ----------------------------------------------
# --------------------------------------------------------------------------------

## USER_SECTION_START 8

## USER_SECTION_END

set_if_not_defined(QT_INSTALL_BASE "C:/Qt")
set_if_not_defined(QT_MAJOR_VERSION 5)
set_if_not_defined(QT_VERSION "autoFind")
set_if_not_defined(QT_COMPILER "autoFind")



## USER_SECTION_START 7

## USER_SECTION_END

# --------------------------------------------------------------------------------
# --------------------------------------------------------------------------------
# --------------------------------------------------------------------------------

project(${LIBRARY_NAME} VERSION ${LIBRARY_VERSION})
 
## USER_SECTION_START 9

## USER_SECTION_END

# Set output paths
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} CACHE PATH "Installation base folder" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")

    set(INSTALL_LIB_PATH "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_LIB_FOLDER}")
    set(INSTALL_BIN_PATH "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_BIN_FOLDER}")
    set(INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_INCLUDE_FOLDER}")
    set(FETCHCONTENT_BASE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dependencies/cache" CACHE PATH "Base directory for FetchContent" FORCE)

    # Sentinel stamp: lives inside CMAKE_BINARY_DIR so it is deleted whenever
    # "Delete Cache and Reconfigure" wipes the binary directory.
    # When the stamp is absent the shared dependency source cache is wiped,
    # forcing a fresh download on the next configure.
    # On a plain reconfigure (cache intact) the stamp exists and the cached
    # source trees are reused without any network access.
    set(_KROIA_STAMP "${CMAKE_BINARY_DIR}/.deps_cache_stamp")
    if(NOT EXISTS "${_KROIA_STAMP}")
        if(EXISTS "${FETCHCONTENT_BASE_DIR}")
            message(STATUS "CMake cache cleared — wiping dependency source cache for fresh download")
            file(REMOVE_RECURSE "${FETCHCONTENT_BASE_DIR}")
        endif()
        file(TOUCH "${_KROIA_STAMP}")
    endif()
    unset(_KROIA_STAMP)

    # Do not re-download or git-fetch dependencies that are already present in
    # FETCHCONTENT_BASE_DIR.  On a plain reconfigure CMake will reuse the
    # existing source trees without touching the network.
    # To force a full re-fetch: use "Delete Cache and Reconfigure" or manually
    # delete the build/…/dependencies/cache directory.
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL
        "Skip git-fetch/update for FetchContent deps already downloaded. Delete cache to force a re-fetch.")

    # Print output paths
    message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    message(STATUS "INSTALL_LIB_PATH: ${INSTALL_LIB_PATH}")
    message(STATUS "INSTALL_BIN_PATH: ${INSTALL_BIN_PATH}")
    message(STATUS "INSTALL_INCLUDE_PATH: ${INSTALL_INCLUDE_PATH}")
    message(STATUS "FETCHCONTENT_BASE_DIR: ${FETCHCONTENT_BASE_DIR}")
else()
    # message(STATUS "This is an included CMakeLists.txt")
endif()

## USER_SECTION_START 10

## USER_SECTION_END


## USER_SECTION_START 4

## USER_SECTION_END

foreach(DEF ${USER_SPECIFIC_GLOBAL_DEFINES})
	add_compile_definitions(${DEF})
endforeach()

## USER_SECTION_START 11

## USER_SECTION_END

include(cmake/QtLocator.cmake)
include(cmake/dependencies.cmake)
include(cmake/ExampleMaster.cmake)

## USER_SECTION_START 12

## USER_SECTION_END

# foreach(DEF ${DEPENDENCY_NAME_MACRO})
#  	add_compile_definitions(${DEF})
# endforeach()

## USER_SECTION_START 5

## USER_SECTION_END

add_subdirectory(core)
if(COMPILE_EXAMPLES AND NOT QSFML_EditorWidget_NO_EXAMPLES)
    message("Include examples for ${LIBRARY_NAME}")
    add_subdirectory(examples)
endif()

## USER_SECTION_START 13

## USER_SECTION_END

if(COMPILE_UNITTESTS AND NOT QSFML_EditorWidget_NO_UNITTESTS)
    message("Include unittests for ${LIBRARY_NAME}")
    add_subdirectory(unittests)
endif()

## USER_SECTION_START 6

## USER_SECTION_END