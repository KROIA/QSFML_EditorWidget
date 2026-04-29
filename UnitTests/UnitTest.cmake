## description: simple library create unit tests

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB)
    # Define the git repository and tag to download from
    set(LIB_NAME UnitTest)		
    set(LIB_MACRO_NAME UNIT_TEST_LIBRARY_AVAILABLE)
    set(GIT_REPO https://github.com/KROIA/UnitTest.git)	
    set(GIT_TAG main)		
	set(NO_EXAMPLES True)	
	set(NO_UNITTESTS True)
	set(ADDITIONAL_INCLUDE_PATHS )

	downloadStandardLibrary()
endfunction()

dep(DEPENDENCY_NAME_MACRO DEPENDENCIES_FOR_SHARED_LIB DEPENDENCIES_FOR_STATIC_LIB DEPENDENCIES_FOR_STATIC_PROFILE_LIB)
