## description: Fast Portable Noise Library - C# C++ C Java HLSL GLSL JavaScript Rust Go 
include(FetchContent)

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB INCLUDE_PATHS)
    # Define the git repository and tag to download from
    set(LIB_NAME FastNeuseLite)
	set(LIB_MACRO_NAME FAST_NOISE_LITE_LIBRARY_AVAILABLE)
    set(GIT_REPO https://github.com/Auburn/FastNoiseLite.git)
    set(GIT_TAG master)

    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG        ${GIT_TAG}
    )

    # Check if the library has already been populated
    FetchContent_GetProperties(${LIB_NAME})
    if(NOT ${LIB_NAME}_ALREADY_POPULATED OR NOT ${LIB_NAME}_SOURCE_DIR OR NOT ${LIB_NAME}_BINARY_DIR)
        message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
        FetchContent_MakeAvailable(${LIB_NAME})
        # Set a persistent cache variable to mark the library as populated
        set(${LIB_NAME}_ALREADY_POPULATED TRUE CACHE INTERNAL "Mark ${LIB_NAME} as populated")
        set(${LIB_NAME}_SOURCE_DIR "${${LIB_NAME}_SOURCE_DIR}" CACHE INTERNAL "${LIB_NAME} source dir")
        set(${LIB_NAME}_BINARY_DIR "${${LIB_NAME}_BINARY_DIR}" CACHE INTERNAL "${LIB_NAME} binary dir")
    else()
        # Re-run MyLibrary's CMakeLists.txt to set up include dirs, libraries, etc.
        add_subdirectory("${${LIB_NAME}_SOURCE_DIR}" "${${LIB_NAME}_BINARY_DIR}" EXCLUDE_FROM_ALL)
    endif()

    # Add this library to the specific profiles of this project
    #list(APPEND DEPS_FOR_SHARED_LIB ${LIB_NAME})
    #list(APPEND DEPS_FOR_STATIC_LIB ${LIB_NAME})
    #list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${LIB_NAME}) # only use for static profiling profile

    # add include path for this library
    # Convert LIB_NAME to lowercase
    string(TOLOWER ${LIB_NAME} LIB_NAME_LOWER)
    list(APPEND INCLUDES ${${LIB_NAME_LOWER}_SOURCE_DIR}/Cpp)

	#set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME}" PARENT_SCOPE)
    #set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    #set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    #set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
    set(${INCLUDE_PATHS} "${${INCLUDE_PATHS}};${INCLUDES}" PARENT_SCOPE)
endfunction()

dep(DEPENDENCY_NAME_MACRO 
    DEPENDENCIES_FOR_SHARED_LIB 
    DEPENDENCIES_FOR_STATIC_LIB 
    DEPENDENCIES_FOR_STATIC_PROFILE_LIB 
    DEPENDENCIES_INCLUDE_PATHS)
