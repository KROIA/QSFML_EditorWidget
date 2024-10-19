## description: EASTL stands for Electronic Arts Standard Template Library. It is an extensive and robust implementation that has an emphasis on high performance. 
include(FetchContent)

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB)
    set(LIB_NAME EASTL)
	set(LIB_MACRO_NAME EASTL)
    set(GIT_REPO https://github.com/electronicarts/EASTL.git)
    set(GIT_TAG master)

    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG        ${GIT_TAG}
    )
    message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
    FetchContent_MakeAvailable(${LIB_NAME})

    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB ${LIB_MACRO_NAME})
    list(APPEND DEPS_FOR_STATIC_LIB ${LIB_MACRO_NAME})
    list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${LIB_MACRO_NAME}) # only use for static profiling profile

	set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME}" PARENT_SCOPE)
    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
endfunction()

dep(DEPENDENCY_NAME_MACRO DEPENDENCIES_FOR_SHARED_LIB DEPENDENCIES_FOR_STATIC_LIB DEPENDENCIES_FOR_STATIC_PROFILE_LIB)
