##
## This file will build the library in different profiles:
##  1. Shared library (dynamic)
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
    else()
        message("ERROR: QT_MODULES is empty. Please specify the required modules or set the variable \"QT_ENABLE\" to OFF")
    endif()
endif()
# end QT settings

GLOB_FILES(H_FILES *.h)
GLOB_FILES(CPP_FILES *.cpp)
GLOB_FILES(C_FILES *.c)
GLOB_FILES(INL_FILES *.inl)


set(SOURCES
    ${CPP_FILES}
    ${C_FILES}
    ${INL_FILES}
    ${H_FILES}
)

## USER_SECTION_START 2

## USER_SECTION_END

if(QT_ENABLE)
    # Search for QT specific files
    GLOB_FILES(UI_FILES *.ui)
    GLOB_FILES(RES_FILES *.qrc)

    # Process .ui files explicitly.  qt_wrap_internal_ui receives the absolute
    # paths produced by GLOB_RECURSE, so it works regardless of where .ui files
    # sit inside core/.  AUTOUIC is NOT enabled on the targets because AUTOUIC
    # searches for .ui files relative to the including .cpp directory and the
    # AUTOUIC_SEARCH_PATHS /... recursive syntax has inconsistent behaviour
    # across CMake versions when the .ui and .cpp are in different sub-trees.
    # Generated ui_*.h files land in CMAKE_CURRENT_BINARY_DIR which is already
    # in every target's include path via $<BUILD_INTERFACE:...>.
    qt_wrap_internal_ui(UIS_HDRS ${UI_FILES})

    # .qrc files stay in SOURCES; AUTORCC (set per-target below) handles them.
    set(SOURCES ${SOURCES}
        ${UIS_HDRS}
        ${RES_FILES})

    # Link the QT modules to your executable
    foreach(MODULE ${QT_MODULES})
        list(APPEND QT_LIBS ${QT_PACKAGE_NAME}::${MODULE})
    endforeach()
    list(APPEND LIBRARY_DEPENDENCIES ${QT_LIBS})
endif()

message("Create library with files:")
foreach(file ${SOURCES})
   message("  File: " ${file})
endforeach()


# ---- Compile definitions ----------------------------------------------------

list(APPEND DEFINES_FOR_ALL_PROFILES
    ${LIB_DEFINE}
    ${DEPENDENCY_NAME_MACRO}
    ${USER_SPECIFIC_DEFINES}
)

if(QT_ENABLE)
    list(APPEND DEFINES_FOR_ALL_PROFILES QT_ENABLED)
    list(FIND QT_MODULES "Widgets" _index)
    if(NOT ${_index} EQUAL -1)
        list(APPEND DEFINES_FOR_ALL_PROFILES QT_WIDGETS_ENABLED)
    endif()
endif()

message("Defines for all profiles:")
foreach(def ${DEFINES_FOR_ALL_PROFILES})
   message("   " ${def})
endforeach()


# ---- Profile definitions ----------------------------------------------------
# Add a new entry to PROFILES here to introduce a fourth profile without
# touching any of the target-creation code below.

set(PROFILES shared static)
if(EASY_PROFILER_IS_AVAILABLE)
    list(APPEND PROFILES static_profile)
endif()

set(TYPE_shared         SHARED)
set(TYPE_static         STATIC)
set(TYPE_static_profile STATIC)

set(DEFS_shared         ${DEFINES_FOR_ALL_PROFILES} BUILD_SHARED)
set(DEFS_static         ${DEFINES_FOR_ALL_PROFILES} BUILD_STATIC)
set(DEFS_static_profile ${DEFINES_FOR_ALL_PROFILES} BUILD_STATIC ${LIB_PROFILE_DEFINE})

set(OUTPUT_shared         ${LIBRARY_NAME})
set(OUTPUT_static         ${LIBRARY_NAME}${STATIC_POSTFIX_STR})
set(OUTPUT_static_profile ${LIBRARY_NAME}${STATIC_POSTFIX_STR}${PROFILING_POSTFIX_STR})

# LIBRARY_DEPENDENCIES (Qt) is PUBLIC: our public headers expose Qt types so
# consumers need Qt too.
# DEPENDENCIES_FOR_* (FetchContent libs) is PRIVATE: they are implementation
# details and must not be pulled into the export set.
set(PRIVATE_DEPS_shared         ${DEPENDENCIES_FOR_SHARED_LIB})
set(PRIVATE_DEPS_static         ${DEPENDENCIES_FOR_STATIC_LIB})
set(PRIVATE_DEPS_static_profile ${DEPENDENCIES_FOR_STATIC_PROFILE_LIB})

message("Extern dependencies for shared lib:           [${DEPENDENCIES_FOR_SHARED_LIB}]")
message("Extern dependencies for static lib:           [${DEPENDENCIES_FOR_STATIC_LIB}]")
if(EASY_PROFILER_IS_AVAILABLE)
    message("Extern dependencies for static profiling lib: [${DEPENDENCIES_FOR_STATIC_PROFILE_LIB}]")
endif()


# ---- Create targets ---------------------------------------------------------

foreach(profile IN LISTS PROFILES)
    set(target ${LIBRARY_NAME}_${profile})

    add_library(${target} ${TYPE_${profile}} ${SOURCES})

    target_compile_definitions(${target} PRIVATE ${DEFS_${profile}})

    set_target_properties(${target} PROPERTIES
        LINKER_LANGUAGE CXX
        OUTPUT_NAME     "${OUTPUT_${profile}}"
        DEBUG_POSTFIX   ${DEBUG_POSTFIX_STR})

    target_link_libraries(${target}
        PUBLIC  ${LIBRARY_DEPENDENCIES}
        PRIVATE ${PRIVATE_DEPS_${profile}})

    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
    )

    if(QT_ENABLE)
        set_target_properties(${target} PROPERTIES
            AUTOMOC ON
            AUTORCC ON)
    endif()
endforeach()


## USER_SECTION_START 3

## USER_SECTION_END

## USER_SECTION_START 4

## USER_SECTION_END

## USER_SECTION_START 5

## USER_SECTION_END


# ---- Install & CMake package export -----------------------------------------
# Only install/export when this library is the top-level cmake project.
# When it is loaded as a FetchContent dependency by another library, these
# commands are skipped — the parent project controls its own install.

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    message("Installation to path: ${INSTALL_LIB_PATH}")

    foreach(profile IN LISTS PROFILES)
        install(TARGETS ${LIBRARY_NAME}_${profile}
                RUNTIME DESTINATION "${INSTALL_LIB_PATH}"
                LIBRARY DESTINATION "${INSTALL_LIB_PATH}"
                ARCHIVE DESTINATION "${INSTALL_LIB_PATH}")
    endforeach()

    copyLibraryHeaders("${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}" "${INSTALL_INCLUDE_PATH}" "${LIBRARY_NAME}")
endif()

## USER_SECTION_START 6

## USER_SECTION_END
