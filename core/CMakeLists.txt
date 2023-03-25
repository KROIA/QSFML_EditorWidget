cmake_minimum_required(VERSION 3.1.0)
# SFML_EditorWidget core 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


include(../QtLocator.cmake)
find_package(Qt5Widgets REQUIRED)

if (Qt5Widgets_FOUND)
    message(STATUS "Using Qt v ${Qt5Widgets_VERSION}")
else()
    message(STATUS "INFO\n\n\tQt5 not found!\n")
endif()


include(FetchContent)
# SFML Shared Lib
set(BUILD_SHARED_LIBS ON)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Dependencies")

FetchContent_Declare(
 SFML
 GIT_REPOSITORY ${SFML_GIT_REPO}
 GIT_TAG ${SFML_GIT_TAG}
)
message("Download dependency: ${SFML_GIT_REPO}")
FetchContent_MakeAvailable(SFML)

set(EASY_PROFILER_NO_GUI True)
FetchContent_Declare(
 EASY_PROFILER
 GIT_REPOSITORY ${EASY_GIT_REPO}
 GIT_TAG ${EASY_GIT_TAG}
)
message("Download dependency: ${EASY_GIT_REPO}")
FetchContent_MakeAvailable(EASY_PROFILER)
#FetchContent_Populate(EASY_PROFILER)
#add_subdirectory(${depname_SOURCE_DIR} ${depname_BINARY_DIR})




if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()



MACRO(FILE_DIRECTORIES return_list ending)
    FILE(GLOB_RECURSE new_list ${ending})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${file_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()


set(CMAKE_AUTOMOC ON) 

# Project sources
#    set(PRJ_HEADERS
#        inc/canvas/CanvasForwardDeclaration.h 
#        inc/canvas/CanvasObjectContainer.h 
#        inc/canvas/CanvasObjectGroup.h 
#        inc/canvas/CanvasSettings.h 
#        inc/canvas/CanvasThreadWorker.h 
#        inc/canvas/RenderLayer.h 
#    
#        inc/components/physik/Collider.h 
#        inc/components/Button.h 
#        inc/components/Component.h 
#        inc/components/Drawable.h 
#        inc/components/DrawableVector.h 
#        inc/components/Line.h 
#        inc/components/SfEventHandle.h 
#        
#    
#    
#    
#       )
#    
#    set(PRJ_QOBJECT_HEADERS
#        inc/canvas/Cabvas.h
#        inc/components/physik/AbstractPressEvent.h 
#        inc/components/KeyPressEvent.h 
#        inc/components/MouseFollower.h 
#        inc/components/MousePressEvent.h 
#    )


FILE_DIRECTORIES(H_FILES *.h)
FILE_DIRECTORIES(CPP_FILES *.cpp)

qt5_wrap_cpp(CPP_MOC_FILES
     ${H_FILES} 
)
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
    ${CPP_MOC_FILES}
    ${CPP_FILES}
    ${H_FILES}
)



message("Create library with files:")
 FOREACH(file ${SOURCES})
    message("  File: " ${file})
 ENDFOREACH()

 set(SFML_EditorWidget_filename "SFML_EditorWidget")
 add_compile_definitions(QSFML_EDITOR_WIDGET_LIB PRIVATE)
if(BUILD_STATIC EQUAL True)
    message("Building SFML_EditorWidget static\n")
    set(SFML_EditorWidget_filename "${SFML_EditorWidget_filename}-s")
	add_compile_definitions(BUILD_STATIC)
    add_library(SFML_EditorWidget STATIC ${SOURCES})
else()
    message("Building SFML_EditorWidget shared\n")
    add_library(SFML_EditorWidget SHARED ${SOURCES})
endif()
message("Buildtype: ${CMAKE_BUILD_TYPE}")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(SFML_EditorWidget_filename "${SFML_EditorWidget_filename}-d")
endif()
set_target_properties(SFML_EditorWidget PROPERTIES LINKER_LANGUAGE CXX  OUTPUT_NAME ${SFML_EditorWidget_filename})
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
