## description: Dear ImGui backend for use with SFML 
include(FetchContent)

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB INCLUDE_PATHS)
    set(LIB_NAME_1 imgui)
    set(LIB_MACRO_NAME_1 IMGUI_LIBRARY_AVAILABLE)
    set(GIT_REPO_1 https://github.com/ocornut/imgui.git)
    set(GIT_TAG_1 master)

    set(LIB_NAME_2 imgui-sfml)
    set(LIB_MACRO_NAME_2 IMGUI_SFML_LIBRARY_AVAILABLE)
    set(GIT_REPO_2 https://github.com/SFML/imgui-sfml.git)
    set(GIT_TAG_2 2.6.x)

    set(LIB_NAME_3 implot)
    set(LIB_MACRO_NAME_3 IMPLOT_LIBRARY_AVAILABLE)
    set(GIT_REPO_3 https://github.com/epezent/implot.git)
    set(GIT_TAG_3 master)

    FetchContent_Declare(
        ${LIB_NAME_1}
        GIT_REPOSITORY ${GIT_REPO_1}
        GIT_TAG        ${GIT_TAG_1}
    )
    FetchContent_Declare(
        ${LIB_NAME_2}
        GIT_REPOSITORY ${GIT_REPO_2}
        GIT_TAG        ${GIT_TAG_2}
    )
    FetchContent_Declare(
		${LIB_NAME_3}
		GIT_REPOSITORY ${GIT_REPO_3}
		GIT_TAG        ${GIT_TAG_3}
	)

    # Check if the library has already been populated
    FetchContent_GetProperties(${LIB_NAME_1})
    if(NOT ${LIB_NAME_1}_ALREADY_POPULATED OR NOT ${LIB_NAME_1}_SOURCE_DIR OR NOT ${LIB_NAME_1}_BINARY_DIR)
        message("Downloading dependency: ${LIB_NAME_1} from: ${GIT_REPO_1} tag: ${GIT_TAG_1}")
        FetchContent_MakeAvailable(${LIB_NAME_1})
        # Set a persistent cache variable to mark the library as populated
        set(${LIB_NAME_1}_ALREADY_POPULATED TRUE CACHE INTERNAL "Mark ${LIB_NAME_1} as populated")
    else()
        # Re-run MyLibrary's CMakeLists.txt to set up include dirs, libraries, etc.
        add_subdirectory("${${LIB_NAME_1}_SOURCE_DIR}" "${${LIB_NAME_1}_BINARY_DIR}" EXCLUDE_FROM_ALL)
    endif()

    set(IMGUI_DIR ${imgui_SOURCE_DIR})
    set(IMGUI_SFML_FIND_SFML OFF)
    set(IMGUI_SFML_IMGUI_DEMO ON)

    # Check if the library has already been populated
    FetchContent_GetProperties(${LIB_NAME_2})
    if(NOT ${LIB_NAME_2}_ALREADY_POPULATED OR NOT ${LIB_NAME_2}_SOURCE_DIR OR NOT ${LIB_NAME_2}_BINARY_DIR)
        message("Downloading dependency: ${LIB_NAME_2} from: ${GIT_REPO_2} tag: ${GIT_TAG_2}")
        FetchContent_MakeAvailable(${LIB_NAME_2})
        # Set a persistent cache variable to mark the library as populated
        set(${LIB_NAME_2}_ALREADY_POPULATED TRUE CACHE INTERNAL "Mark ${LIB_NAME_2} as populated")
    else()
        # Re-run MyLibrary's CMakeLists.txt to set up include dirs, libraries, etc.
        add_subdirectory("${${LIB_NAME_2}_SOURCE_DIR}" "${${LIB_NAME_2}_BINARY_DIR}" EXCLUDE_FROM_ALL)
    endif()

    # Check if the library has already been populated
    FetchContent_GetProperties(${LIB_NAME_3})
    if(NOT ${LIB_NAME_3}_ALREADY_POPULATED OR NOT ${LIB_NAME_3}_SOURCE_DIR OR NOT ${LIB_NAME_3}_BINARY_DIR)
        message("Downloading dependency: ${LIB_NAME_3} from: ${GIT_REPO_3} tag: ${GIT_TAG_3}")
        FetchContent_MakeAvailable(${LIB_NAME_3})
        # Set a persistent cache variable to mark the library as populated
        set(${LIB_NAME_3}_ALREADY_POPULATED TRUE CACHE INTERNAL "Mark ${LIB_NAME_3} as populated")
    else()
        # Re-run MyLibrary's CMakeLists.txt to set up include dirs, libraries, etc.
        add_subdirectory("${${LIB_NAME_3}_SOURCE_DIR}" "${${LIB_NAME_3}_BINARY_DIR}" EXCLUDE_FROM_ALL)
    endif()

    set(IMPLOT_DIR ${implot_SOURCE_DIR})
    # Add ImPlot to the project
    add_library(implot STATIC
        ${IMPLOT_DIR}/implot.cpp
        ${IMPLOT_DIR}/implot_items.cpp
    )
    
    target_include_directories(implot PUBLIC ${IMGUI_DIR})
    target_include_directories(implot PUBLIC ${IMPLOT_DIR})
    target_link_libraries(implot PUBLIC ImGui-SFML::ImGui-SFML)

    
    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB ImGui-SFML::ImGui-SFML implot)
    list(APPEND DEPS_FOR_STATIC_LIB ImGui-SFML::ImGui-SFML implot)
    list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ImGui-SFML::ImGui-SFML implot) # only use for static profiling profile
    list(APPEND INCS ${IMGUI_DIR} ${IMPLOT_DIR}) 

	set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME_1};${LIB_MACRO_NAME_2};${LIB_MACRO_NAME_3}" PARENT_SCOPE)
    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
    set(${INCLUDE_PATHS} "${${INCLUDE_PATHS}};${INCS}" PARENT_SCOPE)
endfunction()

dep(DEPENDENCY_NAME_MACRO DEPENDENCIES_FOR_SHARED_LIB DEPENDENCIES_FOR_STATIC_LIB DEPENDENCIES_FOR_STATIC_PROFILE_LIB DEPENDENCIES_INCLUDE_PATHS)
