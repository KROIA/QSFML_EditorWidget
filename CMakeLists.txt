cmake_minimum_required(VERSION 3.20)
## USER_SECTION_START 1

## USER_SECTION_END


## INFO
## Template version: 1.2.0
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
set(DEBUG_POSTFIX_STR "-d")                    # <AUTO_REPLACED> 
set(STATIC_POSTFIX_STR "-s")                    # <AUTO_REPLACED> 
set(PROFILING_POSTFIX_STR "-p")                 # <AUTO_REPLACED>  

# Langauge settings
set(CMAKE_CXX_STANDARD 14)                      # <AUTO_REPLACED> 
set(CMAKE_CXX_STANDARD_REQUIRED ON)             # <AUTO_REPLACED> 

## USER_SECTION_START 2
set_if_not_defined(CMAKE_CXX_COMPILER "g++")
set_if_not_defined(CMAKE_C_COMPILER "gcc")
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

## USER_SECTION_START 3

## USER_SECTION_END

# --------------------------------------------------------------------------------
# -------------------- Dependencies ----------------------------------------------
# --------------------------------------------------------------------------------

## USER_SECTION_START 8

## USER_SECTION_END

set_if_not_defined(QT_INSTALL_BASE "C:\\Qt")
set_if_not_defined(QT_MAJOR_VERSION 5)
set_if_not_defined(QT_VERSION "autoFind")
set_if_not_defined(QT_COMPILER "autoFind")



## USER_SECTION_START 7

## USER_SECTION_END

# --------------------------------------------------------------------------------
# --------------------------------------------------------------------------------
# --------------------------------------------------------------------------------

project(${LIBRARY_NAME})
 
# Set output paths
set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")

set(INSTALL_LIB_PATH "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_LIB_FOLDER}")
set(INSTALL_BIN_PATH "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_BIN_FOLDER}")
set(INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_INCLUDE_FOLDER}")
set(FETCHCONTENT_BASE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dependencies/cache")

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

## USER_SECTION_START 4

## USER_SECTION_END


include(cmake/QtLocator.cmake)
include(cmake/dependencies.cmake)
include(cmake/ExampleMaster.cmake)

## USER_SECTION_START 5

## USER_SECTION_END

add_subdirectory(core)
if(COMPILE_EXAMPLES AND NOT QSFML_EditorWidget_NO_EXAMPLES)
    message("Include examples for ${LIBRARY_NAME}")
    add_subdirectory(examples)
endif()
if(COMPILE_UNITTESTS AND NOT QSFML_EditorWidget_NO_UNITTESTS)
    message("Include unittests for ${LIBRARY_NAME}")
    add_subdirectory(unitTests)
endif()

## USER_SECTION_START 6

## USER_SECTION_END