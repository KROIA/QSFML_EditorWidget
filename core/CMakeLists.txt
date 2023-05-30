cmake_minimum_required(VERSION 3.1.0)
# SFML_EditorWidget core 
set(SFML_EditorWidget_filename "SFML_EditorWidget")

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


set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dependencies")

# SFML Static Lib
set(SFML_STATIC_LIBRARIES TRUE)
set(BUILD_SHARED_LIBS OFF)
set(SFML_USE_STATIC_LIBS ON)
FetchContent_Declare(
 SFML
 GIT_REPOSITORY ${SFML_GIT_REPO}
 GIT_TAG ${SFML_GIT_TAG}
)
message("Download dependency: ${SFML_GIT_REPO}")
FetchContent_MakeAvailable(SFML)


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
qt5_wrap_cpp(CPP_MOC_FILES ${H_FILES})

set(SFML_EditorWidget_Include "inc")

set(SOURCES
    ${CPP_MOC_FILES}
    ${CPP_FILES}
    ${H_FILES}
)



message("Create library with files:")
 FOREACH(file ${SOURCES})
    message("  File: " ${file})
 ENDFOREACH()

 
 set(SFML_EditorWidget_dependencies 
    Qt5::Widgets
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-system
    sfml-window)

add_library(SFML_EditorWidget_shared SHARED ${SOURCES})
add_library(SFML_EditorWidget_static STATIC ${SOURCES})
add_library(SFML_EditorWidget_static_profile STATIC ${SOURCES})


target_compile_definitions(sfml-graphics PRIVATE  SFML_STATIC)
target_compile_definitions(sfml-audio PRIVATE  SFML_STATIC)
target_compile_definitions(sfml-network PRIVATE  SFML_STATIC)
target_compile_definitions(sfml-system PRIVATE  SFML_STATIC)
target_compile_definitions(sfml-window PRIVATE  SFML_STATIC)
target_compile_definitions(easy_profiler PRIVATE  EASY_PROFILER_STATIC)
target_compile_definitions(SFML_EditorWidget_shared PRIVATE  QSFML_EDITOR_WIDGET_LIB)
target_compile_definitions(SFML_EditorWidget_static PRIVATE  BUILD_STATIC QSFML_EDITOR_WIDGET_LIB)
target_compile_definitions(SFML_EditorWidget_static_profile PRIVATE QSFML_PROFILING BUILD_STATIC QSFML_EDITOR_WIDGET_LIB)
#if(QSFML_PROFILING)
#    target_compile_definitions(SFML_EditorWidget_shared PRIVATE  QSFML_PROFILING)
#    target_compile_definitions(SFML_EditorWidget_static PRIVATE  QSFML_PROFILING)
#    set(SFML_EditorWidget_dependencies 
#        ${SFML_EditorWidget_dependencies}
#        easy_profiler)
#ENDIF()


set_target_properties(easy_profiler PROPERTIES DEBUG_POSTFIX "-d")
set_target_properties(SFML_EditorWidget_shared PROPERTIES LINKER_LANGUAGE CXX  OUTPUT_NAME "${SFML_EditorWidget_filename}" DEBUG_POSTFIX "-d")
set_target_properties(SFML_EditorWidget_static PROPERTIES LINKER_LANGUAGE CXX  OUTPUT_NAME "${SFML_EditorWidget_filename}-s" DEBUG_POSTFIX "-d")
set_target_properties(SFML_EditorWidget_static_profile PROPERTIES LINKER_LANGUAGE CXX  OUTPUT_NAME "${SFML_EditorWidget_filename}-s-p" DEBUG_POSTFIX "-d")




target_link_libraries(SFML_EditorWidget_shared ${SFML_EditorWidget_dependencies})
target_link_libraries(SFML_EditorWidget_static ${SFML_EditorWidget_dependencies})
target_link_libraries(SFML_EditorWidget_static_profile ${SFML_EditorWidget_dependencies} easy_profiler)

target_include_directories(SFML_EditorWidget_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
target_include_directories(SFML_EditorWidget_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
target_include_directories(SFML_EditorWidget_static_profile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
# Define the install path
message("Installation to path: ${CMAKE_INSTALL_PREFIX}/bin")
message("Installation to path: ${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS SFML_EditorWidget_shared DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(TARGETS SFML_EditorWidget_shared DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS SFML_EditorWidget_static DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS SFML_EditorWidget_static_profile DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS easy_profiler DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
install(TARGETS sfml-graphics sfml-audio sfml-network sfml-system sfml-window DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")
