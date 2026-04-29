include(FetchContent)

# ---------------------------------------------------------------------------
# Offline / local dependency mode
#
# Set USE_LOCAL_DEPENDENCIES=ON and LOCAL_DEPENDENCIES_PATH to a directory
# that contains one sub-folder per dependency (named exactly as LIB_NAME).
# When the flag is ON and the folder exists, FetchContent uses SOURCE_DIR
# instead of downloading — no internet access required.
#
# LOCAL_DEPENDENCIES_PATH may be absolute or relative.
# Relative paths are resolved from CMAKE_SOURCE_DIR (the project root folder
# that contains the top-level CMakeLists.txt), NOT from the build directory.
#
# These variables are declared ONLY in the top-level project.  When this
# library is loaded as a FetchContent dependency the variables are already
# in the CMake cache (set by the top-level project's cmake args or presets)
# and must not be re-declared — re-declaring would reset the default to OFF
# if the cache entry somehow got cleared, hiding the top-level setting.
# All transitive dependencies automatically inherit the top-level values
# because cache variables are global.
#
# Examples (CMakePresets.json or command line):
#   -DUSE_LOCAL_DEPENDENCIES=ON -DLOCAL_DEPENDENCIES_PATH="C:/Projects/TemplateTests"
#   -DUSE_LOCAL_DEPENDENCIES=ON -DLOCAL_DEPENDENCIES_PATH="../"
# ---------------------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    option(USE_LOCAL_DEPENDENCIES
        "Use local source folders instead of downloading dependencies from git" OFF)
    set(LOCAL_DEPENDENCIES_PATH "" CACHE PATH
        "Root folder containing local dependency clones (one sub-folder per LIB_NAME). Absolute or relative to the project root.")
endif()

# Macro to search for files with a given extension.
# call:
#   GLOB_FILES(H_FILES *.h)
#
# All matching files will be saved in the H_FILES variable.
# CONFIGURE_DEPENDS causes CMake to re-run configure when the file list changes.
MACRO(GLOB_FILES return_list ending)
    FILE(GLOB_RECURSE ${return_list} CONFIGURE_DEPENDS ${ending})
ENDMACRO()


function(get_filename_from_path FILE_PATH FILE_NAME_VAR)
    get_filename_component(${FILE_NAME_VAR} ${FILE_PATH} NAME)
endfunction()


# Function name: windeployqt
# Params: targetName        Name of the target created using add_executable(...)
#         outputPath        Path where the deployment will be done
#     
function(windeployqt targetName outputPath)
    
    # check if QT_PATH is empty
    if (NOT QT_PATH)
		message("QT_PATH is not set. include QtLocator.cmake first, to find a qt installation or assign a 
                 QT path to it. example: set(QT_PATH \"C:/Qt/5.14.2\")")
        return()
    endif()

    set(targetExePath "${outputPath}/${targetName}.exe") 
    set(DEPLOY_COMMAND  "${QT_PATH}/bin/windeployqt.exe 
		--no-compiler-runtime 
		--translations de,en
		--no-system-d3d-compiler 
		--no-opengl-sw 
		--pdb 
		--dir \"${outputPath}\" \"${targetExePath}\"
        --qmldir \"${CMAKE_SOURCE_DIR}\"")

	set(CMD "${DEPLOY_COMMAND}")
	string(REPLACE "\\" "/" CMD "${CMD}")

	
	install(
    CODE
    "execute_process(
        COMMAND
        ${CMD}
    )"
)
	
endfunction()


function(copyLibraryHeaders headerRootFolder destinationPath destinationFolderName)
    install(DIRECTORY "${headerRootFolder}/"
            DESTINATION "${destinationPath}/${destinationFolderName}"
            FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
    message("Installing headers from: ${headerRootFolder} to ${destinationPath}/${destinationFolderName}")
endfunction()


function(set_if_not_defined varName value)
    if(NOT DEFINED ${varName})
        set(${varName} ${value} PARENT_SCOPE)
    endif()
endfunction()






# Macro to declare a FetchContent dependency with local/cached/git fallback.
# Use this instead of raw FetchContent_Declare() when you need to call
# FetchContent_MakeAvailable() separately (e.g. when ordering between deps matters).
#
# Call:
#   smartDeclare(<LIB_NAME> <GIT_REPO> <GIT_TAG>)
#
# Then call FetchContent_MakeAvailable(<LIB_NAME>) when ready.
# Skips the Declare entirely if the library is already populated.
# Respects USE_LOCAL_DEPENDENCIES and LOCAL_DEPENDENCIES_PATH.
macro(smartDeclare LIB_NAME GIT_REPO GIT_TAG)
    FetchContent_GetProperties(${LIB_NAME})
    string(TOLOWER "${LIB_NAME}" _sfd_name_lower)
    if(NOT ${_sfd_name_lower}_POPULATED)
        if(NOT "${LOCAL_DEPENDENCIES_PATH}" STREQUAL "" AND NOT IS_ABSOLUTE "${LOCAL_DEPENDENCIES_PATH}")
            get_filename_component(_sfd_resolved_path
                "${CMAKE_SOURCE_DIR}/${LOCAL_DEPENDENCIES_PATH}" ABSOLUTE)
        else()
            set(_sfd_resolved_path "${LOCAL_DEPENDENCIES_PATH}")
        endif()

        if(USE_LOCAL_DEPENDENCIES AND NOT "${_sfd_resolved_path}" STREQUAL ""
                AND EXISTS "${_sfd_resolved_path}/${LIB_NAME}")
            message("Using local dependency: ${LIB_NAME} from: ${_sfd_resolved_path}/${LIB_NAME}")
            FetchContent_Declare(
                ${LIB_NAME}
                SOURCE_DIR "${_sfd_resolved_path}/${LIB_NAME}"
            )
        elseif(DEFINED FETCHCONTENT_BASE_DIR
                AND EXISTS "${FETCHCONTENT_BASE_DIR}/${_sfd_name_lower}-src")
            message("Using cached dependency: ${LIB_NAME} (${FETCHCONTENT_BASE_DIR}/${_sfd_name_lower}-src)")
            FetchContent_Declare(
                ${LIB_NAME}
                SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/${_sfd_name_lower}-src"
            )
        else()
            message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
            FetchContent_Declare(
                ${LIB_NAME}
                GIT_REPOSITORY ${GIT_REPO}
                GIT_TAG        ${GIT_TAG}
            )
        endif()
    endif()
endmacro()




# Macro to download and setup a default library using FetchContent.
# A default library is a library that was created using this project's template or is compatible with this macro.
# 
# Params:
#   LIB_NAME: Name of the library (used for FetchContent)
#   LIB_MACRO_NAME: Macro name to be added to the LIBRARY_MACRO_NAME variable
#   GIT_REPO: Git repository URL of the library
#   GIT_TAG: Git tag or commit hash to checkout
#   NO_EXAMPLES: Set to True to disable building examples of the library
#   NO_UNITTTESTS: Set to True to disable building unittests of the library
#   ADDITIONAL_INCLUDE_PATHS: Additional include paths to be added for this library
#   ADDITIONAL_SHARED_LIB_DEPENDENCIES: Additional libraries to be linked for the shared library profile
#   ADDITIONAL_STATIC_LIB_DEPENDENCIES: Additional libraries to be linked for the static library profile
#   ADDITIONAL_STATIC_PROFILE_LIB_DEPENDENCIES: Additional libraries to be linked for the static profiling library profile
#
#   LIBRARY_MACRO_NAME: passed variable by the caller to accumulate all library macro names
#   SHARED_LIB: passed variable by the caller to accumulate all shared libraries
#   STATIC_LIB: passed variable by the caller to accumulate all static libraries
#   STATIC_PROFILE_LIB: passed variable by the caller to accumulate all static profiling libraries
#   INCLUDE_PATHS: passed variable by the caller to accumulate all include paths
#
# Example call from inside a dependency cmake file:
#[[
## description: This description will be used as tooltip
include(FetchContent)
include(cmake/utility.cmake)

function(dep LIBRARY_MACRO_NAME SHARED_LIB STATIC_LIB STATIC_PROFILE_LIB INCLUDE_PATHS)
    # Define the git repository and tag to download from
    set(LIB_NAME AppSettings)								
    set(LIB_MACRO_NAME APP_SETTINGS_LIBRARY_AVAILABLE)		
    set(GIT_REPO https://github.com/KROIA/AppSettings.git)	
    set(GIT_TAG main)										
    set(NO_EXAMPLES True)	
    set(NO_UNITTESTS True)
    set(ADDITIONAL_INCLUDE_PATHS )

    downloadStandardLibrary()
endfunction()

dep(DEPENDENCY_NAME_MACRO
    DEPENDENCIES_FOR_SHARED_LIB 
    DEPENDENCIES_FOR_STATIC_LIB 
    DEPENDENCIES_FOR_STATIC_PROFILE_LIB 
    DEPENDENCIES_INCLUDE_PATHS)
#]]
macro(downloadStandardLibrary)
    set(${LIB_NAME}_NO_EXAMPLES  ${NO_EXAMPLES})
    set(${LIB_NAME}_NO_UNITTESTS ${NO_UNITTESTS})

    # Check if this library was already populated by an earlier call (diamond dependency).
    # If it was, skip FetchContent_Declare entirely so the top-level project's
    # declaration (local path or git) always wins, and no duplicate messages appear.
    FetchContent_GetProperties(${LIB_NAME})
    string(TOLOWER "${LIB_NAME}" _ldc_name_lower)
    if(NOT ${_ldc_name_lower}_POPULATED)
        # Resolve LOCAL_DEPENDENCIES_PATH: relative paths are taken from CMAKE_SOURCE_DIR
        # (the project root), not from the build directory or the current source dir.
        if(NOT "${LOCAL_DEPENDENCIES_PATH}" STREQUAL "" AND NOT IS_ABSOLUTE "${LOCAL_DEPENDENCIES_PATH}")
            get_filename_component(_ldc_resolved_path
                "${CMAKE_SOURCE_DIR}/${LOCAL_DEPENDENCIES_PATH}" ABSOLUTE)
        else()
            set(_ldc_resolved_path "${LOCAL_DEPENDENCIES_PATH}")
        endif()

        # Priority 1: explicit local override
        # Priority 2: already downloaded into FETCHCONTENT_BASE_DIR — reuse without git ops
        # Priority 3: download from git
        if(USE_LOCAL_DEPENDENCIES AND NOT "${_ldc_resolved_path}" STREQUAL ""
                AND EXISTS "${_ldc_resolved_path}/${LIB_NAME}")
            message("Using local dependency: ${LIB_NAME} from: ${_ldc_resolved_path}/${LIB_NAME}")
            FetchContent_Declare(
                ${LIB_NAME}
                SOURCE_DIR "${_ldc_resolved_path}/${LIB_NAME}"
            )
        elseif(DEFINED FETCHCONTENT_BASE_DIR
                AND EXISTS "${FETCHCONTENT_BASE_DIR}/${_ldc_name_lower}-src")
            message("Using cached dependency: ${LIB_NAME} (${FETCHCONTENT_BASE_DIR}/${_ldc_name_lower}-src)")
            FetchContent_Declare(
                ${LIB_NAME}
                SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/${_ldc_name_lower}-src"
            )
        else()
            message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
            FetchContent_Declare(
                ${LIB_NAME}
                GIT_REPOSITORY ${GIT_REPO}
                GIT_TAG        ${GIT_TAG}
            )
        endif()
    endif()
    FetchContent_MakeAvailable(${LIB_NAME})

    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB ${LIB_NAME}_shared ${ADDITIONAL_SHARED_LIB_DEPENDENCIES})
    list(APPEND DEPS_FOR_STATIC_LIB ${LIB_NAME}_static ${ADDITIONAL_STATIC_LIB_DEPENDENCIES})

    # Only link the profile variant if the dependency actually built one
    if(TARGET ${LIB_NAME}_static_profile)
        list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${LIB_NAME}_static_profile ${ADDITIONAL_STATIC_PROFILE_LIB_DEPENDENCIES})
    else()
        list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${LIB_NAME}_static ${ADDITIONAL_STATIC_PROFILE_LIB_DEPENDENCIES})
    endif()

    # Auto-generate LIB_MACRO_NAME from LIB_NAME when the caller did not set it.
    # e.g. Logger → LOGGER_LIBRARY_AVAILABLE, imgui-sfml → IMGUI_SFML_LIBRARY_AVAILABLE
    if(NOT DEFINED LIB_MACRO_NAME OR "${LIB_MACRO_NAME}" STREQUAL "")
        string(TOUPPER "${LIB_NAME}" _ldc_auto_macro)
        string(REPLACE "-" "_" _ldc_auto_macro "${_ldc_auto_macro}")
        set(LIB_MACRO_NAME "${_ldc_auto_macro}_LIBRARY_AVAILABLE")
    endif()

    set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME}" PARENT_SCOPE)
    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
    set(${INCLUDE_PATHS} "${${INCLUDE_PATHS}};${ADDITIONAL_INCLUDE_PATHS}" PARENT_SCOPE)
endmacro()






# Macro to download and setup an external library using FetchContent.
# This macro can be used for libraries that do not follow the default library structure of this project.
# Params:
#   LIB_NAME: Name of the library (used for FetchContent)
#   LIB_MACRO_NAME: Macro name to be added to the LIBRARY_MACRO_NAME variable
#   GIT_REPO: Git repository URL of the library
#   GIT_TAG: Git tag or commit hash to checkout
#   SHARED_LIB_DEPENDENCY: The library name to be used for the shared library profile. If not set, LIB_NAME will be used. 
#   STATIC_LIB_DEPENDENCY: The library name to be used for the static library profile. If not set, LIB_NAME will be used.
#   STATIC_PROFILE_LIB_DEPENDENCY: The library name to be used for the static profiling library profile. If not set, LIB_NAME will be used.
#   ADDITIONAL_SHARED_LIB_DEPENDENCIES: Additional libraries to be linked for the shared library profile
#   ADDITIONAL_STATIC_LIB_DEPENDENCIES: Additional libraries to be linked for the static library profile
#   ADDITIONAL_STATIC_PROFILE_LIB_DEPENDENCIES: Additional libraries to be linked for the static profiling library profile
#   ADDITIONAL_INCLUDE_PATHS: Additional include paths to be added for this library
#
#   LIBRARY_MACRO_NAME: passed variable by the caller to accumulate all library macro names
#   SHARED_LIB: passed variable by the caller to accumulate all shared libraries
#   STATIC_LIB: passed variable by the caller to accumulate all static libraries
#   STATIC_PROFILE_LIB: passed variable by the caller to accumulate all static profiling libraries
#   INCLUDE_PATHS: passed variable by the caller to accumulate all include paths
macro(downloadExternalLibrary)
    FetchContent_GetProperties(${LIB_NAME})
    string(TOLOWER "${LIB_NAME}" _ldc_name_lower)
    if(NOT ${_ldc_name_lower}_POPULATED)
        # Resolve LOCAL_DEPENDENCIES_PATH: relative paths are taken from CMAKE_SOURCE_DIR.
        if(NOT "${LOCAL_DEPENDENCIES_PATH}" STREQUAL "" AND NOT IS_ABSOLUTE "${LOCAL_DEPENDENCIES_PATH}")
            get_filename_component(_ldc_resolved_path
                "${CMAKE_SOURCE_DIR}/${LOCAL_DEPENDENCIES_PATH}" ABSOLUTE)
        else()
            set(_ldc_resolved_path "${LOCAL_DEPENDENCIES_PATH}")
        endif()

        if(USE_LOCAL_DEPENDENCIES AND NOT "${_ldc_resolved_path}" STREQUAL ""
                AND EXISTS "${_ldc_resolved_path}/${LIB_NAME}")
            message("Using local dependency (external): ${LIB_NAME} from: ${_ldc_resolved_path}/${LIB_NAME}")
            FetchContent_Declare(
                ${LIB_NAME}
                SOURCE_DIR "${_ldc_resolved_path}/${LIB_NAME}"
            )
        elseif(DEFINED FETCHCONTENT_BASE_DIR
                AND EXISTS "${FETCHCONTENT_BASE_DIR}/${_ldc_name_lower}-src")
            message("Using cached dependency (external): ${LIB_NAME} (${FETCHCONTENT_BASE_DIR}/${_ldc_name_lower}-src)")
            FetchContent_Declare(
                ${LIB_NAME}
                SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/${_ldc_name_lower}-src"
            )
        else()
            message("Downloading dependency (external): ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
            FetchContent_Declare(
                ${LIB_NAME}
                GIT_REPOSITORY ${GIT_REPO}
                GIT_TAG        ${GIT_TAG}
            )
        endif()
    endif()
    FetchContent_MakeAvailable(${LIB_NAME})

    
    if (NOT DEFINED SHARED_LIB_DEPENDENCY)
        set(SHARED_LIB_DEPENDENCY ${LIB_NAME})
    endif()

    if (NOT DEFINED STATIC_LIB_DEPENDENCY)
        set(STATIC_LIB_DEPENDENCY ${LIB_NAME})
    endif()

    if (NOT DEFINED STATIC_PROFILE_LIB_DEPENDENCY)
        set(STATIC_PROFILE_LIB_DEPENDENCY ${LIB_NAME})
    endif()

    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB ${SHARED_LIB_DEPENDENCY} ${ADDITIONAL_SHARED_LIB_DEPENDENCIES})
    list(APPEND DEPS_FOR_STATIC_LIB ${STATIC_LIB_DEPENDENCY} ${ADDITIONAL_STATIC_LIB_DEPENDENCIES})
    list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${STATIC_PROFILE_LIB_DEPENDENCY} ${ADDITIONAL_STATIC_PROFILE_LIB_DEPENDENCIES}) # only use for static profiling profile

    # Auto-generate LIB_MACRO_NAME from LIB_NAME when the caller did not set it.
    # e.g. SFML → SFML_LIBRARY_AVAILABLE, imgui-sfml → IMGUI_SFML_LIBRARY_AVAILABLE
    if(NOT DEFINED LIB_MACRO_NAME OR "${LIB_MACRO_NAME}" STREQUAL "")
        string(TOUPPER "${LIB_NAME}" _ldc_auto_macro)
        string(REPLACE "-" "_" _ldc_auto_macro "${_ldc_auto_macro}")
        set(LIB_MACRO_NAME "${_ldc_auto_macro}_LIBRARY_AVAILABLE")
    endif()

    set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME}" PARENT_SCOPE)
    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
    set(${INCLUDE_PATHS} "${${INCLUDE_PATHS}};${ADDITIONAL_INCLUDE_PATHS}" PARENT_SCOPE)
endmacro()

