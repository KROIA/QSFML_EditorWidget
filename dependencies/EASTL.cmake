## description: EASTL stands for Electronic Arts Standard Template Library. It is an extensive and robust implementation that has an emphasis on high performance. 
include(FetchContent)

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB)
    set(LIB_NAME EASTL)
	set(LIB_MACRO_NAME EASTL_LIBRARY_AVAILABLE)
    set(GIT_REPO https://github.com/electronicarts/EASTL.git)
    set(GIT_TAG master)

    downloadExternalLibrary()
endfunction()

dep(DEPENDENCY_NAME_MACRO DEPENDENCIES_FOR_SHARED_LIB DEPENDENCIES_FOR_STATIC_LIB DEPENDENCIES_FOR_STATIC_PROFILE_LIB)
