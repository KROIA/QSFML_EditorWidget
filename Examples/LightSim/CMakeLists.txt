
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)   
string(REPLACE " " "_" ProjectId ${PROJECT_NAME})
if(QSFML_PROFILING)
    set(PROJECT_NAME ${PROJECT_NAME}_profile)
endif()
project(${PROJECT_NAME})


# Macro to search for files with given file ending.
# call:
#   FILE_DIRECTORIES(H_FILES *.h)
#
# all *.h files will be saved in the H_FILES variable
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



include_directories(inc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) 
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})

include(${QT_LOCATOR_CMAKE})
find_package(Qt5Widgets REQUIRED)

# Get all source files
FILE_DIRECTORIES(H_FILES *.h)
FILE_DIRECTORIES(CPP_FILES *.cpp)
FILE_DIRECTORIES(UI_FILES *.ui)


qt5_wrap_ui(UIS_HDRS ${UI_FILES})
qt_wrap_cpp(SRC_HDRS ${H_FILES})

set(Files 
	${H_FILES}
	${CPP_FILES}
	${UIS_HDRS}
	${SRC_HDRS})

 add_executable(${PROJECT_NAME} ${Files})

if(QSFML_PROFILING)
    target_link_libraries(${PROJECT_NAME} SFML_EditorWidget_static_profile)
else()
    target_link_libraries(${PROJECT_NAME} SFML_EditorWidget_static)
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_STATIC)


install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_PATH}/bin/windeployqt.exe"
            --no-compiler-runtime
            --no-translations
            --no-system-d3d-compiler
            --no-opengl-sw
            --no-angle
            --no-webkit2
            --dir "${CMAKE_INSTALL_PREFIX}/bin"
            --pdb
            "$<TARGET_FILE:${PROJECT_NAME}>"
		COMMENT "Running windeployqt..."
    )
endif()
