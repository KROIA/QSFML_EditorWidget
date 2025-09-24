include(FetchContent)

# Macro to search for files with given file ending.
# call:
#   FILE_DIRECTORIES(H_FILES *.h)
#
# all *.h files will be saved in the H_FILES variable
MACRO(FILE_DIRECTORIES return_list ending)
    FILE(GLOB_RECURSE new_list ${ending})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${file_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
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
		--no-translations 
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
    # Copy the folder
    file(COPY ${headerRootFolder}
         DESTINATION ${CMAKE_BINARY_DIR})

    get_filename_component(FOLDER_NAME ${headerRootFolder} NAME)
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/${destinationFolderName}")

    # Rename the copied folder
    file(RENAME ${CMAKE_BINARY_DIR}/${FOLDER_NAME}
                ${CMAKE_BINARY_DIR}/${destinationFolderName})

    # Install the modified folder
    install(DIRECTORY ${CMAKE_BINARY_DIR}/${destinationFolderName}
            DESTINATION ${destinationPath})

    message("Installing headers from: ${headerRootFolder} to ${destinationPath}/${destinationFolderName}")

endfunction()


function(set_if_not_defined varName value)
    if(NOT DEFINED ${varName})
        set(${varName} ${value} PARENT_SCOPE)
    endif()
endfunction()






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
    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG        ${GIT_TAG}
    )

    set(${LIB_NAME}_NO_EXAMPLES ${NO_EXAMPLES})	
    set(${LIB_NAME}_NO_UNITTESTS ${NO_UNITTESTS})

    message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
    FetchContent_MakeAvailable(${LIB_NAME})

    # Add this library to the specific profiles of this project
    list(APPEND DEPS_FOR_SHARED_LIB ${LIB_NAME}_shared ${ADDITIONAL_SHARED_LIB_DEPENDENCIES})
    list(APPEND DEPS_FOR_STATIC_LIB ${LIB_NAME}_static ${ADDITIONAL_STATIC_LIB_DEPENDENCIES})
    list(APPEND DEPS_FOR_STATIC_PROFILE_LIB ${LIB_NAME}_static_profile ${ADDITIONAL_STATIC_PROFILE_LIB_DEPENDENCIES}) # only use for static profiling profile

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
    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG        ${GIT_TAG}
    )

    message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
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

    set(${LIBRARY_MACRO_NAME} "${${LIBRARY_MACRO_NAME}};${LIB_MACRO_NAME}" PARENT_SCOPE)
    set(${SHARED_LIB} "${${SHARED_LIB}};${DEPS_FOR_SHARED_LIB}" PARENT_SCOPE)
    set(${STATIC_LIB} "${${STATIC_LIB}};${DEPS_FOR_STATIC_LIB}" PARENT_SCOPE)
    set(${STATIC_PROFILE_LIB} "${${STATIC_PROFILE_LIB}};${DEPS_FOR_STATIC_PROFILE_LIB}" PARENT_SCOPE)
    set(${INCLUDE_PATHS} "${${INCLUDE_PATHS}};${ADDITIONAL_INCLUDE_PATHS}" PARENT_SCOPE)
endmacro()

