## 
## This file will build the library in 3 different ways:
##  1. Shared library
##  2. Static library
##  3. Static library with profiling using easy_profiler
##     This option is only available if the easy_profiler library is added to the dependencies
##

## USER_SECTION_START 1
cmake_minimum_required(VERSION 3.20)
## USER_SECTION_END

set(INCLUDE_PATH "inc")

set(LIBRARY_NAME_STATIC ${LIBRARY_NAME}_static)
set(LIBRARY_NAME_SHARED ${LIBRARY_NAME}_shared)
if(EASY_PROFILER_IS_AVAILABLE)
    set(LIBRARY_NAME_STATIC_PROFILE ${LIBRARY_NAME_STATIC}_profile)
endif()

# QT settings
if(QT_ENABLE)
    message("Using QT modules: ${QT_MODULES}")
    list(LENGTH QT_MODULES list_length)
    if(NOT list_length EQUAL 0)
        find_package(${QT_PACKAGE_NAME} REQUIRED COMPONENTS ${QT_MODULES})

        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        #set(CMAKE_AUTOUIC ON)
    else()
        message("ERROR: QT_MODULES is empty. Please specify the required modules or set the variable \"QT_ENABLE\" to OFF")
    endif()
endif()
# end QT settings

include_directories(${INCLUDE_PATH})

FILE_DIRECTORIES(H_FILES *.h)
FILE_DIRECTORIES(CPP_FILES *.cpp)


set(SOURCES
    ${CPP_FILES}
    ${H_FILES}
)

## USER_SECTION_START 2

## USER_SECTION_END

if(QT_ENABLE)
    # Search for QT specific files
    FILE_DIRECTORIES(UI_FILES *.ui)    
    FILE_DIRECTORIES(RES_FILES *.qrc)    

    # Identify headers containing Q_OBJECT macros
    set(MOC_HEADERS "")
    foreach(HEADER ${H_FILES})
        file(READ ${H_FILES} HEADER_CONTENTS)
        if(HEADER_CONTENTS MATCHES "Q_OBJECT")
            list(APPEND MOC_HEADERS ${H_FILES})
        endif()
    endforeach()

    # Use qt5_wrap_cpp to generate moc files for QObject classes
    qt_wrap_internal_cpp(CPP_MOC_FILES ${MOC_HEADERS})
    qt_wrap_internal_ui(UIS_HDRS ${UI_FILES})
    qt_add_internal_resources(RESOURCE_FILES ${RES_FILES})

    set(SOURCES ${SOURCES}
	    ${CPP_MOC_FILES}
	    ${UIS_HDRS}
        ${RESOURCE_FILES})

    # Link the QT modules to your executable
    foreach(MODULE ${QT_MODULES})
        set(QT_LIBS ${QT_LIBS} ${QT_PACKAGE_NAME}::${MODULE})
    endforeach()
    set(LIBRARY_DEPENDENCIES ${LIBRARY_DEPENDENCIES} ${QT_LIBS})
endif()

message("Create library with files:")
FOREACH(file ${SOURCES})
   message("  File: " ${file})
ENDFOREACH()


# Set defines for inside the code of the library
list(APPEND DEFINES_FOR_ALL_PROFILES ${LIB_DEFINE})

# Add the names of the dependencies as a define
foreach(DEPENDENCY ${DEPENDENCY_NAME_MACRO})
	list(APPEND DEFINES_FOR_ALL_PROFILES ${DEPENDENCY})
endforeach()

foreach(DEF ${USER_SPECIFIC_DEFINES})
	list(APPEND DEFINES_FOR_ALL_PROFILES ${DEF})
endforeach()

if(QT_ENABLE)
    list(APPEND DEFINES_FOR_ALL_PROFILES QT_ENABLED)
    # Check if QT_MODULES contains Widgets
    list(FIND QT_MODULES "Widgets" _index)

    if(NOT ${_index} EQUAL -1)
        list(APPEND DEFINES_FOR_ALL_PROFILES QT_WIDGETS_ENABLED)
    endif()
endif()

message("Defines for all profiles: ${DEFINES_FOR_ALL_PROFILES}")
list(APPEND DEFINES_FOR_SHARED_LIB ${DEFINES_FOR_ALL_PROFILES} BUILD_SHARED)
list(APPEND DEFINES_FOR_STATIC_LIB ${DEFINES_FOR_ALL_PROFILES} BUILD_STATIC)
if(EASY_PROFILER_IS_AVAILABLE)
    list(APPEND DEFINES_FOR_STATIC_PROFILE_LIB ${DEFINES_FOR_ALL_PROFILES} BUILD_STATIC ${LIB_PROFILE_DEFINE})
endif()


add_library(${LIBRARY_NAME_SHARED}                          SHARED      ${SOURCES})
add_library(${LIBRARY_NAME_STATIC}                          STATIC      ${SOURCES})
if(EASY_PROFILER_IS_AVAILABLE)
    add_library(${LIBRARY_NAME_STATIC_PROFILE}                  STATIC      ${SOURCES})
endif()
 
target_compile_definitions(${LIBRARY_NAME_SHARED}           PRIVATE     ${DEFINES_FOR_SHARED_LIB})
target_compile_definitions(${LIBRARY_NAME_STATIC}           PRIVATE     ${DEFINES_FOR_STATIC_LIB})
if(EASY_PROFILER_IS_AVAILABLE)
    target_compile_definitions(${LIBRARY_NAME_STATIC_PROFILE}   PRIVATE     ${DEFINES_FOR_STATIC_PROFILE_LIB})
endif()

set_target_properties(${LIBRARY_NAME_SHARED}                PROPERTIES  LINKER_LANGUAGE CXX  OUTPUT_NAME "${LIBRARY_NAME}"                                              DEBUG_POSTFIX ${DEBUG_POSTFIX_STR})
set_target_properties(${LIBRARY_NAME_STATIC}                PROPERTIES  LINKER_LANGUAGE CXX  OUTPUT_NAME "${LIBRARY_NAME}${STATIC_POSTFIX_STR}"                         DEBUG_POSTFIX ${DEBUG_POSTFIX_STR})
if(EASY_PROFILER_IS_AVAILABLE)
    set_target_properties(${LIBRARY_NAME_STATIC_PROFILE}        PROPERTIES  LINKER_LANGUAGE CXX  OUTPUT_NAME "${LIBRARY_NAME}${STATIC_POSTFIX_STR}${PROFILING_POSTFIX_STR}" DEBUG_POSTFIX ${DEBUG_POSTFIX_STR})
endif()

## USER_SECTION_START 3

## USER_SECTION_END

# External dependencies from the dependencies folder
message("Extern dependencies for shared lib: [${DEPENDENCIES_FOR_SHARED_LIB}]")
message("Extern dependencies for static lib: [${DEPENDENCIES_FOR_STATIC_LIB}]")
if(EASY_PROFILER_IS_AVAILABLE)
    message("Extern dependencies for static profiling lib: [${DEPENDENCIES_FOR_STATIC_PROFILE_LIB}]")
endif()

target_link_libraries(${LIBRARY_NAME_SHARED}            ${LIBRARY_DEPENDENCIES} ${DEPENDENCIES_FOR_SHARED_LIB})
target_link_libraries(${LIBRARY_NAME_STATIC}            ${LIBRARY_DEPENDENCIES} ${DEPENDENCIES_FOR_STATIC_LIB})
if(EASY_PROFILER_IS_AVAILABLE)
    target_link_libraries(${LIBRARY_NAME_STATIC_PROFILE}    ${LIBRARY_DEPENDENCIES} ${DEPENDENCIES_FOR_STATIC_PROFILE_LIB})
endif()

## USER_SECTION_START 4

## USER_SECTION_END

target_include_directories(${LIBRARY_NAME_SHARED} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
    ${CMAKE_CURRENT_BINARY_DIR}
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
target_include_directories(${LIBRARY_NAME_STATIC}  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
    ${CMAKE_CURRENT_BINARY_DIR}
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
if(EASY_PROFILER_IS_AVAILABLE)
target_include_directories(${LIBRARY_NAME_STATIC_PROFILE} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
    ${CMAKE_CURRENT_BINARY_DIR}
	# $<INSTALL_INTERFACE:inc>  # <prefix>/inc 
)
endif()

## USER_SECTION_START 5

## USER_SECTION_END

# Define the install path
message("Installation to path: ${INSTALL_LIB_PATH}")
if(EASY_PROFILER_IS_AVAILABLE)
    install(TARGETS ${LIBRARY_NAME_SHARED} ${LIBRARY_NAME_STATIC}  ${LIBRARY_NAME_STATIC_PROFILE} DESTINATION "${INSTALL_LIB_PATH}")
else()
    install(TARGETS ${LIBRARY_NAME_SHARED} ${LIBRARY_NAME_STATIC} DESTINATION "${INSTALL_LIB_PATH}")
endif()

copyLibraryHeaders("${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}" "${INSTALL_INCLUDE_PATH}" "${LIBRARY_NAME}")

## USER_SECTION_START 6

## USER_SECTION_END