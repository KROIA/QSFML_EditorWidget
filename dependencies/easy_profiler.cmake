## description: simple profiler for applications
include(FetchContent)

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB)
    # Define the git repository and tag to download from
    set(LIB_NAME easy_profiler)
    set(LIB_MACRO_NAME EASY_PROFILER_LIBRARY_AVAILABLE)
    set(GIT_REPO https://github.com/yse/easy_profiler.git)
    set(GIT_TAG v2.1.0)

    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG        ${GIT_TAG}
    )

    set(EASY_PROFILER_NO_SAMPLES True)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build easy_profiler as static library.")

    # Change the QT version to V5 because easy_profiler is not compatible with QT6
    if(NOT QT_MAJOR_VERSION EQUAL 5)
        set(QT_MAJOR_VERSION 5)
        set(QT_VERSION "autoFind")
        set(QT_MISSING True)
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtLocator.cmake)
    endif()

    # Check if the library has already been populated
    FetchContent_GetProperties(${LIB_NAME})
    if(NOT ${LIB_NAME}_ALREADY_POPULATED)
        message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
        FetchContent_MakeAvailable(${LIB_NAME})
        # Set a persistent cache variable to mark the library as populated
        set(${LIB_NAME}_ALREADY_POPULATED TRUE CACHE INTERNAL "Mark ${LIB_NAME} as populated")
    else()
        # Re-run MyLibrary's CMakeLists.txt to set up include dirs, libraries, etc.
        add_subdirectory("${${LIB_NAME}_SOURCE_DIR}" "${${LIB_NAME}_BINARY_DIR}" EXCLUDE_FROM_ALL)
    endif()

    set(EASY_PROFILER_IS_AVAILABLE ON PARENT_SCOPE)


    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB )
    list(APPEND DEPS_FOR_STATIC_LIB )
    list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${LIB_NAME}) # only use for static profiling profile

    # Deploy the Profiler GUI
    if(QT_ENABLE AND QT_DEPLOY)
        windeployqt(profiler_gui ${INSTALL_BIN_PATH})
    endif()


    #set(EASY_PROFILER_NO_GUI False)
    set_target_properties(${LIB_NAME} PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(${LIB_NAME} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(${LIB_NAME} PROPERTIES CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX ${DEBUG_POSTFIX_STR})
    target_compile_definitions(${LIB_NAME} PUBLIC EASY_PROFILER_STATIC)

    set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME}" PARENT_SCOPE)
    # Save the local dependencies to the parent scope
    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
endfunction()

dep(DEPENDENCY_NAME_MACRO DEPENDENCIES_FOR_SHARED_LIB DEPENDENCIES_FOR_STATIC_LIB DEPENDENCIES_FOR_STATIC_PROFILE_LIB)
