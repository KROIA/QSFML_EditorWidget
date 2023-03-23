cmake_minimum_required(VERSION 3.1.0)



set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH C:/Qt)
include(QtLocator.cmake)
# finally, use Qt5 + COMPONENTS technique, compatible with Qt_DIR
MESSAGE("Qt5Config.cmake path: "${Qt5_DIR})
#FIND_PACKAGE(Qt5 COMPONENTS Core Widgets REQUIRED)
#find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt5Widgets REQUIRED)
if (Qt5Widgets_FOUND)
    message(STATUS "Using Qt v${Qt5Widgets_VERSION}")
else()
    message(STATUS "INFO\n\n\tQt5 not found!\n")
endif()


include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
 SFML
 GIT_REPOSITORY https://github.com/SFML/SFML.git
 GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
 EASY_PROFILER
 GIT_REPOSITORY https://github.com/yse/easy_profiler.git
 GIT_TAG v2.1.0
)
FetchContent_MakeAvailable(EASY_PROFILER)




if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()



MACRO(HEADER_DIRECTORIES return_list ending)
    FILE(GLOB_RECURSE new_list ${ending})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${file_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(H_FILES *.h)
HEADER_DIRECTORIES(CPP_FILES *.cpp)

# Sources
#file(GLOB CPP_FILES
#     "src/*.cpp"
#)

# Headers
set(SFML_EditorWidget_Include "inc")
#file(GLOB H_FILES
#     "${SFML_EditorWidget_Include}/*.h"
#)

set(SOURCES
    ${CPP_FILES}
    ${H_FILES}
)

 FOREACH(file ${SOURCES})
    message("File: "${file})
 ENDFOREACH()

message("Create library with files: \n${${SOURCES}}")
if(BUILD_STATIC)
    message("Building SFML_EditorWidget static\n")
    add_library(SFML_EditorWidget STATIC ${SOURCES})
else()
    message("Building SFML_EditorWidget shared\n")
    add_library(SFML_EditorWidget SHARED ${SOURCES})
endif()
set_target_properties(SFML_EditorWidget PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(SFML_EditorWidget 
    Qt5::Widgets
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-system
    sfml-window
    easy_profiler)

target_include_directories(easy_profiler PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc>  # <prefix>/inc
)
