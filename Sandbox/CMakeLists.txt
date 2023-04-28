
project(Sandbox)


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

include(../QtLocator.cmake)
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

add_executable(Sandbox ${Files})


target_link_libraries(Sandbox SFML_EditorWidget_static)
target_compile_definitions(Sandbox PUBLIC BUILD_STATIC)


install(TARGETS Sandbox DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
if(WIN32)
    add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND "${QT_PATH}/bin/windeployqt.exe"
            --no-compiler-runtime
            --no-translations
            --no-system-d3d-compiler
            --no-opengl-sw
            --no-angle
            --no-webkit2
            --dir "${CMAKE_INSTALL_PREFIX}/bin"
            --pdb
            "$<TARGET_FILE:Sandbox>"
		COMMENT "Running windeployqt..."
    )
endif()
