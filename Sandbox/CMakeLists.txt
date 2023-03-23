
project(Sandbox)

include_directories(inc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) 

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

 qt5_wrap_ui(UIS_HDRS
     ${UIS} 
 )

set(Files 
	${H_Files}
	${SRC_Files}
	${UIS_HDRS})



add_executable(Sandbox ${Files})
target_link_libraries(Sandbox SFML_EditorWidget)