
project(Sandbox)

include_directories(inc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) 
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})

include(../QtLocator.cmake)
find_package(Qt5Widgets REQUIRED)

set(H_Files 
	inc/AABBDisplayer.h 
	inc/DrawableVector.h 
	inc/sandbox.h 
	inc/SandboxObject.h)

set(SRC_Files 
	src/main.cpp 
	src/sandbox.cpp 
	src/SandboxObject.cpp)

set ( UIS
	sandbox.ui
)

qt5_wrap_ui(UIS_HDRS ${UIS})

qt_wrap_cpp(SRC_HDRS ${H_Files})

set(Files 
	${H_Files}
	${SRC_Files}
	${UIS_HDRS}
	${SRC_HDRS})

add_executable(Sandbox ${Files})
if(${BUILD_STATIC})
	#add_compile_definitions(BUILD_STATIC)
	target_link_libraries(Sandbox SFML_EditorWidget_static)
	target_compile_definitions(Sandbox PUBLIC BUILD_STATIC)
else()
	target_link_libraries(Sandbox SFML_EditorWidget_shared)
endif()


