cmake_minimum_required(VERSION 3.15)

project(SFML_EditorWidget)

# Dependencies:
# QT base Path
#   Set a specific QT msvc compiler path 
#   If not defined, the QtLocator.cmake try's to search for the highest QT5 version to use
# set(QT_PATH C:/Qt/5.11.0/msvc2017_64)

#  SFML
set(SFML_GIT_REPO  https://github.com/SFML/SFML.git)
set(SFML_GIT_TAG   2.5.1)

#  EasyProfiler
set(EASY_GIT_REPO  https://github.com/yse/easy_profiler.git)
set(EASY_GIT_TAG   v2.1.0)

#
# -----------------------------------------
#
# Check if variable is set, otherwise set to default
if(NOT RELATIVE_BUILD_FOLDER)
	set(RELATIVE_BUILD_FOLDER build)
endif()
if(NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
endif()
if(NOT RELATIVE_INSTALL_FOLDER)
	set(RELATIVE_INSTALL_FOLDER lib)
endif()
 
set(CMAKE_GENERATOR "Visual Studio 17 2022")


# Set output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")




link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})




add_subdirectory(core)


add_subdirectory(Sandbox)


#add_executable(SFML_EditorWidget_test main.cpp)
#target_link_libraries(SFML_EditorWidget_test SFML_EditorWidget)

