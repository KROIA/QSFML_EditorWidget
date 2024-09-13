## description: SFML: Simple and fast multimedia library
include(FetchContent)

function(dep SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB)
    # Define the git repository and tag to download from
    set(LIB_NAME SFML)
    set(GIT_REPO https://github.com/SFML/SFML.git)
    set(GIT_TAG 2.5.1)

    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG        ${GIT_TAG}
    )
    # Specific SFML settings
    # SFML Static Lib
    set(SFML_MISC_INSTALL_PREFIX "${INSTALL_LIB_PATH}/build/misc") 
    set(SFML_STATIC_LIBRARIES TRUE)
    set(BUILD_SHARED_LIBS OFF)
    set(SFML_USE_STATIC_LIBS ON)


    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static library.")
    message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
    FetchContent_MakeAvailable(${LIB_NAME})

    target_compile_definitions(sfml-graphics	PRIVATE		SFML_STATIC)
    target_compile_definitions(sfml-audio		PRIVATE		SFML_STATIC)
    target_compile_definitions(sfml-network		PRIVATE		SFML_STATIC)
    target_compile_definitions(sfml-system		PRIVATE		SFML_STATIC)
    target_compile_definitions(sfml-window		PRIVATE		SFML_STATIC)

    list(APPEND SFML_DEPS 
	    sfml-graphics
        sfml-audio
        sfml-network
        sfml-system
        sfml-window)

    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB ${SFML_DEPS})
    list(APPEND DEPS_FOR_STATIC_LIB ${SFML_DEPS})
    list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${SFML_DEPS}) # only use for static profiling profile


    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
endfunction()

dep(DEPENDENCIES_FOR_SHARED_LIB DEPENDENCIES_FOR_STATIC_LIB DEPENDENCIES_FOR_STATIC_PROFILE_LIB)
