# Credits: 
# https://stackoverflow.com/questions/15639781/how-to-find-the-qt5-cmake-module-on-windows
#
# Note:
# The variable QT_INSTALL_BASE must be set before this script gets executed
# The variable points to the base directory where all versions of QT are installed. Default C:\Qt
# If the variable is empty or not defined, the script try's to search for the installation of the QtCreator to find the 
# installation folder.
#

function(get_newest_msvc_compiler_path out rootPath)
    FILE(GLOB CompilerPaths "${rootPath}/msvc*")
    set(NewestYear 0)
    foreach(CompilerPath ${CompilerPaths})
        get_filename_component(CompilerVersion ${CompilerPath} NAME) # Extract the version from the path
    
        # Extract year from the version string
        string(REGEX MATCH "[0-9][0-9][0-9][0-9]" YearMatch ${CompilerVersion})
        if (YearMatch)
            set(Year ${CMAKE_MATCH_0})
            if (Year GREATER NewestYear)
                set(NewestYear ${Year})
                set(newest ${CompilerPath})
            endif()
        endif()
    endforeach()
    set(${out} ${newest} PARENT_SCOPE)
endfunction()

# Function to extract the version number from a path
function(get_version_number out path)
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" version ${path})
    set(${out} ${version} PARENT_SCOPE)
endfunction()


set_if_not_defined(QT_MISSING True)

IF(QT_QMAKE_EXECUTABLE AND QT_VERSION STREQUAL "autoFind") # C:/Qt/6.5.2/msvc2019_64/bin/qmake.exe
    # Find the position of "qmake.exe" in the string
    string(FIND ${QT_QMAKE_EXECUTABLE} "qmake.exe" QMAKE_POS)

    if(QMAKE_POS EQUAL -1)
        message("The string 'qmake.exe' is not present in QT_QMAKE_EXECUTABLE")
    else()
        # Find the position of the first "/"
        string(FIND ${QT_QMAKE_EXECUTABLE} "/" FIRST_SLASH_POS REVERSE)

        # Extract the substring up to the position of the first "/"
        string(SUBSTRING ${QT_QMAKE_EXECUTABLE} 0 ${FIRST_SLASH_POS} FIRST_PATH)
        string(FIND ${FIRST_PATH} "/" FIRST_SLASH_POS REVERSE)
        string(SUBSTRING ${FIRST_PATH} 0 ${FIRST_SLASH_POS} FIRST_PATH)
        SET(QT_PATH ${FIRST_PATH})
        string(FIND ${QT_PATH} "/" FIRST_SLASH_POS2 REVERSE)
        MATH(EXPR FIRST_SLASH_POS2 "${FIRST_SLASH_POS2}+1")
        string(SUBSTRING ${FIRST_PATH} ${FIRST_SLASH_POS2} ${FIRST_SLASH_POS}  FIRST_PATH)
        SET(QT_COMPILER ${FIRST_PATH})



        get_version_number(QT_VERSION ${QT_PATH})
        # Extract the major version
        string(REGEX MATCH "([0-9]+)" QT_MAJOR_VERSION ${QT_VERSION})

        SET(QT_MISSING False)
    endif()

ENDIF()


if(NOT QT_VERSION STREQUAL "autoFind" AND DEFINED QT_VERSION)
    if(NOT EXISTS ${QT_INSTALL_BASE}/${QT_VERSION})
        message(FATAL_ERROR "Can't find QT installation. Path: ${QT_INSTALL_BASE}/${QT_VERSION} does not exist")
    endif()

    message("Using predefined Qt Version: ${QT_VERSION}")
    SET(QT_MISSING False)
    # Extract the major version number using a regular expression
    string(REGEX MATCH "([0-9]+)" QT_MAJOR_VERSION ${QT_VERSION})

    # Convert the extracted version number to an integer
    math(EXPR QT_MAJOR_VERSION "${QT_MAJOR_VERSION}")

    if(NOT DEFINED QT_COMPILER OR QT_COMPILER STREQUAL "autoFind")
        get_newest_msvc_compiler_path(QT_PATH ${QT_INSTALL_BASE}/${QT_VERSION})
    else()
        SET(QT_PATH "${QT_INSTALL_BASE}/${QT_VERSION}/${QT_COMPILER}")
    endif()
endif()

if(NOT DEFINED QT_MAJOR_VERSION)
    SET(QT_MAJOR_VERSION 5) # Default Qt5 version
endif()

message("QT_MAJOR_VERSION = ${QT_MAJOR_VERSION}")
SET(QT_PACKAGE_NAME Qt${QT_MAJOR_VERSION})
SET(QT_WIDGET_PACKAGE_NAME Qt${QT_MAJOR_VERSION}Widgets)



IF(QT_MISSING)
    MESSAGE("Searching for QT installs...")

    if(NOT DEFINED QT_INSTALL_BASE OR "${QT_INSTALL_BASE}" STREQUAL "")
        # look for user-registry pointing to qtcreator
        GET_FILENAME_COMPONENT(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.cpp\\shell\\Open\\Command] PATH)
         # get root path so we can search for 5.3, 5.4, 5.5, etc
        STRING(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
        LIST(GET QT_BIN 0 QT_INSTALL_BASE)
    endif()

    # get root path so we can search for 5.3, 5.4, 5.5, etc
    FILE(GLOB QT_VERSIONS "${QT_INSTALL_BASE}/${QT_MAJOR_VERSION}.*")
    
    # Create a list of version numbers
	set(version_numbers )
    foreach(path ${QT_VERSIONS})
        get_version_number(version ${path})
        list(APPEND version_numbers ${version})
    endforeach()

    # Sort the list of paths in descending order based on their version number
    list(LENGTH version_numbers num_versions)

    # Compare versions and find the newest one
    math(EXPR last_index "${num_versions} - 1")
    foreach(i RANGE ${last_index})
        foreach(j RANGE ${last_index})
            list(GET version_numbers ${i} version_i)
            list(GET version_numbers ${j} version_j)

            if(version_i VERSION_GREATER version_j)
                list(GET QT_VERSIONS ${i} path_i)
                list(GET QT_VERSIONS ${j} path_j)
                list(REMOVE_AT QT_VERSIONS ${i})
                list(INSERT QT_VERSIONS ${i} ${path_j})
                list(REMOVE_AT QT_VERSIONS ${j})
                list(INSERT QT_VERSIONS ${j} ${path_i})
                list(REMOVE_AT version_numbers ${i})
                list(INSERT version_numbers ${i} ${version_j})
                list(REMOVE_AT version_numbers ${j})
                list(INSERT version_numbers ${j} ${version_i})
            endif()
        endforeach()
    endforeach()
    list(GET QT_VERSIONS 0 newestQtVersionPath)

    # fix any double slashes which seem to be common
    STRING(REPLACE "//" "/"  newestQtVersionPath "${newestQtVersionPath}")



    # Initialize variables to store the newest compiler version and path
    
    set(NewestCompilerPath "${newestQtVersionPath}/${QT_COMPILER}")

    if(NOT DEFINED QT_COMPILER OR NOT EXISTS ${NewestCompilerPath} OR QT_COMPILER STREQUAL "autoFind")
        get_newest_msvc_compiler_path(NewestCompilerPath ${newestQtVersionPath})
    endif()

    

    if (EXISTS ${NewestCompilerPath})
        set(QT_PATH ${NewestCompilerPath})
        SET(QT_MISSING False)
    endif()
ENDIF()




# use Qt_DIR approach so you can find Qt after cmake has been invoked
IF(NOT QT_MISSING)
    if (EXISTS ${QT_PATH})
        
        SET(Qt${QT_MAJOR_VERSION}_DIR "${QT_PATH}/lib/cmake/Qt${QT_MAJOR_VERSION}")
        SET(Qt${QT_MAJOR_VERSION}Widgets_DIR  "${QT_PATH}/lib/cmake/Qt${QT_MAJOR_VERSION}Widgets")
        SET(Qt${QT_MAJOR_VERSION}Test_DIR "${QT_PATH}/lib/cmake/Qt${QT_MAJOR_VERSION}Test")
        
        MESSAGE("Qt${QT_MAJOR_VERSION}Config.cmake path:  ${Qt${QT_MAJOR_VERSION}_DIR}")
    else()
        message(FATAL_ERROR "No QT${QT_MAJOR_VERSION} installation found. \n"
                            "Searching for compiler: ${QT_PATH}")
    endif()
ENDIF()


message("QT_PATH: ${QT_PATH}")
message("QT_COMPILER: ${QT_COMPILER}")
message("QT_VERSION: ${QT_VERSION}")
message("QT_MAJOR_VERSION: ${QT_MAJOR_VERSION}")

function(qt_wrap_internal_cpp outFiles)
    cmake_parse_arguments(inFiles "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(inFiles ${inFiles_UNPARSED_ARGUMENTS})
    # Get the length of the list
    list(LENGTH inFiles listLength)
    if(listLength GREATER  0)
        # Call qt_wrap_cpp with the parsed arguments
        if(${QT_MAJOR_VERSION} EQUAL 5)
            qt5_wrap_cpp(${outFiles} ${inFiles})
        elseif(${QT_MAJOR_VERSION} EQUAL 6)
            qt6_wrap_cpp(${outFiles} ${inFiles})
        endif()

        # Export the output files variable for parent scope
        set(${outFiles} ${${outFiles}} PARENT_SCOPE)
    endif()
endfunction()


function(qt_wrap_internal_ui outFiles)
    cmake_parse_arguments(inFiles "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(inFiles ${inFiles_UNPARSED_ARGUMENTS})
    # Get the length of the list
    list(LENGTH inFiles listLength)
    if(listLength GREATER  0)
        # Call qt_wrap_cpp with the parsed arguments
        if(${QT_MAJOR_VERSION} EQUAL 5)
            qt5_wrap_ui(${outFiles} ${inFiles})
        elseif(${QT_MAJOR_VERSION} EQUAL 6)
            qt6_wrap_ui(${outFiles} ${inFiles})
        endif()

        # Export the output files variable for parent scope
        set(${outFiles} ${${outFiles}} PARENT_SCOPE)
    endif()
endfunction()

function(qt_add_internal_resources outFiles)
    cmake_parse_arguments(inFiles "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(inFiles ${inFiles_UNPARSED_ARGUMENTS})
    # Get the length of the list
    list(LENGTH inFiles listLength)
    if(listLength GREATER  0)
        # Call qt_wrap_cpp with the parsed arguments
        if(${QT_MAJOR_VERSION} EQUAL 5)
            qt5_add_resources(${outFiles} ${inFiles})
        elseif(${QT_MAJOR_VERSION} EQUAL 6)
            qt6_add_resources(${outFiles} ${inFiles})
        endif()

        # Export the output files variable for parent scope
        set(${outFiles} ${${outFiles}} PARENT_SCOPE)
    endif()
endfunction()
