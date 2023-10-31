cmake_minimum_required(VERSION 3.1.0)



# External dependencies
include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(DEBUG_POSTFIX_STR "-d")

function(use_easy_profiler libsContainer)
	#  EasyProfiler
	set(EASY_GIT_REPO  https://github.com/yse/easy_profiler.git)
	set(EASY_GIT_TAG   v2.1.0)


	FetchContent_Declare(
		EASY_PROFILER
		GIT_REPOSITORY	${EASY_GIT_REPO}
		GIT_TAG			${EASY_GIT_TAG}
	)
	message("Download dependency: ${EASY_GIT_REPO}")

	set(EASY_PROFILER_NO_SAMPLES True)
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build easy_profiler as static library.")
	FetchContent_MakeAvailable(EASY_PROFILER)


	#set(EASY_PROFILER_NO_GUI False)
	set_target_properties(easy_profiler PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
	set_target_properties(easy_profiler PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
	set_target_properties(easy_profiler PROPERTIES CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
	set_target_properties(easy_profiler PROPERTIES DEBUG_POSTFIX ${DEBUG_POSTFIX_STR})
	target_compile_definitions(easy_profiler PUBLIC EASY_PROFILER_STATIC)

	#
	set(${libsContainer} ${${libsContainer}} "easy_profiler" PARENT_SCOPE)
endfunction()




function(use_sfml libsContainer)
	#  SFML
	set(SFML_GIT_REPO  https://github.com/SFML/SFML.git)
	set(SFML_GIT_TAG   2.5.1)

	# SFML Static Lib
	set(SFML_MISC_INSTALL_PREFIX "${INSTALL_LIB_PATH}/misc") 
	set(SFML_STATIC_LIBRARIES TRUE)
	set(BUILD_SHARED_LIBS OFF)
	set(SFML_USE_STATIC_LIBS ON)
	FetchContent_Declare(
		SFML
		GIT_REPOSITORY ${SFML_GIT_REPO}
		GIT_TAG ${SFML_GIT_TAG}
    )
	message("Download dependency: ${SFML_GIT_REPO}")

	set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static library.")
	FetchContent_MakeAvailable(SFML)

	target_compile_definitions(sfml-graphics	PRIVATE		SFML_STATIC)
	target_compile_definitions(sfml-audio		PRIVATE		SFML_STATIC)
	target_compile_definitions(sfml-network		PRIVATE		SFML_STATIC)
	target_compile_definitions(sfml-system		PRIVATE		SFML_STATIC)
	target_compile_definitions(sfml-window		PRIVATE		SFML_STATIC)
	
	#install(TARGETS sfml-graphics sfml-audio sfml-network sfml-system sfml-window DESTINATION "${CMAKE_INSTALL_PREFIX}/${RELATIVE_INSTALL_FOLDER}")

	#
	set(${libsContainer} ${${libsContainer}} 
	sfml-graphics
    sfml-audio
    sfml-network
    sfml-system
    sfml-window
	PARENT_SCOPE)
endfunction()
# End external dependencies