#   # CMakeList.txt: CMake-Projekt für "CMAKE". Schließen Sie die Quelle ein, und definieren Sie
#   # projektspezifische Logik hier.
#   #
#   cmake_minimum_required (VERSION 3.8)
#   
#   # Enable Hot Reload for MSVC compilers if supported.
#   if (POLICY CMP0141)
#     cmake_policy(SET CMP0141 NEW)
#     set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#   endif()
#   
#   project ("SFML_EditorWidget")
#   
#   # Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
#   add_executable(SFML_EditorWidget main.cpp)
#   
#   if (CMAKE_VERSION VERSION_GREATER 3.12)
#     set_property(TARGET SFML_EditorWidget PROPERTY CXX_STANDARD 20)
#   endif()
#   
#   # TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.


cmake_minimum_required(VERSION 3.15)

project(MyProject)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

#add_subdirectory(Submodules/EasyProfiler)


# include(FetchContent)
# set(BUILD_SHARED_LIBS OFF)
# FetchContent_Declare(
#  SFML
#  GIT_REPOSITORY https://github.com/SFML/SFML.git
#  GIT_TAG 2.5.1
# )
# FetchContent_MakeAvailable(SFML)
# 
# FetchContent_Declare(
#  EASY_PROFILER
#  GIT_REPOSITORY https://github.com/yse/easy_profiler.git
#  GIT_TAG v2.1.0
# )
# FetchContent_MakeAvailable(EASY_PROFILER)
# 
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED true)

set(BUILD_STATIC True)

add_compile_definitions(QSFML_EDITOR_WIDGET_LIB)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

if(BUILD_STATIC)
	add_compile_definitions(BUILD_STATIC)
endif()


add_subdirectory(core)
add_subdirectory(Sandbox)


add_executable(MyProject main.cpp)
target_link_libraries(MyProject SFML_EditorWidget)

