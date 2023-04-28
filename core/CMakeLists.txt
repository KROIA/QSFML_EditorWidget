cmake_minimum_required(VERSION 3.1.0)
# SFML_EditorWidget core 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


include(../QtLocator.cmake)
message("Search for QT5 finished")
find_package(Qt5Widgets REQUIRED)
message("find_package for Qt5Widgets finished")

if (Qt5Widgets_FOUND)
    message(STATUS "Using Qt v ${Qt5Widgets_VERSION}")
else()
    message(STATUS "INFO\n\n\tQt5 not found!\n")
endif()


include(FetchContent)
# SFML Shared Lib
set(BUILD_SHARED_LIBS ON)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dependencies")

FetchContent_Declare(
 SFML
 GIT_REPOSITORY ${SFML_GIT_REPO}
 GIT_TAG ${SFML_GIT_TAG}
)
message("Download dependency: ${SFML_GIT_REPO}")
FetchContent_MakeAvailable(SFML)
message("Download dependency: ${SFML_GIT_REPO} done")

set(EASY_PROFILER_NO_GUI False)
FetchContent_Declare(
 EASY_PROFILER
 GIT_REPOSITORY ${EASY_GIT_REPO}
 GIT_TAG ${EASY_GIT_TAG}
)
message("Download dependency: ${EASY_GIT_REPO}")

FetchContent_MakeAvailable(EASY_PROFILER)



#add_custom_command(
#    TARGET easy_profiler POST_BUILD VERBATIM
#    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${easy_profiler}>" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#)

#install(TARGETS easy_profiler DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")


set_target_properties(easy_profiler PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set_target_properties(easy_profiler PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set_target_properties(easy_profiler PROPERTIES CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")

#set_target_properties(easy_profiler PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
#set_target_properties(easy_profiler PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

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
 #add_compile_definitions(QSFML_EDITOR_WIDGET_LIB PRIVATE)
 #message(${BUILD_STATIC})
if(${BUILD_STATIC})
    target_compile_definitions(easy_profiler PRIVATE  BUILD_SHARED_LIBS)
    #message("Building SFML_EditorWidget static\n")
    #set(SFML_EditorWidget_filename "${SFML_EditorWidget_filename}-s")
	#add_compile_definitions(BUILD_STATIC)
    #add_library(SFML_EditorWidget_static STATIC ${SOURCES})
    #target_compile_definitions(SFML_EditorWidget_static BUILD_STATIC)

#else()
    #message("Building SFML_EditorWidget shared\n")
    #add_library(SFML_EditorWidget SHARED ${SOURCES})
endif()
#message("Buildtype: ${CMAKE_BUILD_TYPE}")

add_library(SFML_EditorWidget_shared SHARED ${SOURCES})
add_library(SFML_EditorWidget_static STATIC ${SOURCES})

target_compile_definitions(SFML_EditorWidget_shared PRIVATE  QSFML_EDITOR_WIDGET_LIB)
target_compile_definitions(SFML_EditorWidget_static PRIVATE  BUILD_STATIC QSFML_EDITOR_WIDGET_LIB)


set_target_properties(SFML_EditorWidget_shared PROPERTIES LINKER_LANGUAGE CXX  OUTPUT_NAME "${SFML_EditorWidget_filename}" DEBUG_POSTFIX "-d")
set_target_properties(SFML_EditorWidget_static PROPERTIES LINKER_LANGUAGE CXX  OUTPUT_NAME "${SFML_EditorWidget_filename}-s" DEBUG_POSTFIX "-d")

set(SFML_EditorWidget_dependencies 
    Qt5::Widgets
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-system
    sfml-window
    easy_profiler)


target_link_libraries(SFML_EditorWidget_shared ${SFML_EditorWidget_dependencies})
target_link_libraries(SFML_EditorWidget_static ${SFML_EditorWidget_dependencies})

target_include_directories(SFML_EditorWidget_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
target_include_directories(SFML_EditorWidget_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
# Define the install path
install(TARGETS SFML_EditorWidget_shared DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(TARGETS SFML_EditorWidget_shared DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS SFML_EditorWidget_static DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS easy_profiler DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS sfml-graphics sfml-audio sfml-network sfml-system sfml-window DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
