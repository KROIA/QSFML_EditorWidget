# Credits: 
# https://stackoverflow.com/questions/15639781/how-to-find-the-qt5-cmake-module-on-windows
#
# Note:
# The variable QT_INSTALL_BASE must be set before this script gets executed
# The variable points to the base directory where all versions of QT are installed. Default C:\Qt
# If the variable is empty or not defined, the script try's to search for the installation of the QtCreator to find the 
# installation folder.
#

SET(QT_MISSING True)


# Function to extract the version number from a path
function(get_version_number out path)
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" version ${path})
    set(${out} ${version} PARENT_SCOPE)
endfunction()

# msvc only; mingw will need different logic
IF(MSVC AND QT_MISSING)
    MESSAGE("Searching for QT installs...")

    if(NOT DEFINED QT_INSTALL_BASE OR "${QT_INSTALL_BASE}" STREQUAL "")
        # look for user-registry pointing to qtcreator
        GET_FILENAME_COMPONENT(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.cpp\\shell\\Open\\Command] PATH)
         # get root path so we can search for 5.3, 5.4, 5.5, etc
        STRING(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
        LIST(GET QT_BIN 0 QT_INSTALL_BASE)
    endif()

    # get root path so we can search for 5.3, 5.4, 5.5, etc
    FILE(GLOB QT_VERSIONS "${QT_INSTALL_BASE}/5.*")
    
    # Create a list of version numbers
	set(version_numbers )
    foreach(path ${QT_VERSIONS})
        get_version_number(version ${path})
        #message("Extracted version: "${version})
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
    list(GET QT_VERSIONS 0 QT_VERSION)

    # fix any double slashes which seem to be common
    STRING(REPLACE "//" "/"  QT_VERSION "${QT_VERSION}")

    FILE(GLOB CompilerPaths "${QT_VERSION}/msvc*")


    # Initialize variables to store the newest compiler version and path
    set(NewestYear 0)
    set(NewestCompilerPath "")

    foreach(CompilerPath ${CompilerPaths})
        get_filename_component(CompilerVersion ${CompilerPath} NAME) # Extract the version from the path

        # Extract year from the version string
        string(REGEX MATCH "[0-9][0-9][0-9][0-9]" YearMatch ${CompilerVersion})
        if (YearMatch)
            set(Year ${CMAKE_MATCH_0})
            if (Year GREATER NewestYear)
                set(NewestYear ${Year})
                set(NewestCompilerPath ${CompilerPath})
            endif()
        endif()
    endforeach()


    message("Newest MSVC Compiler Version: ${NewestCompilerVersion}")
    message("Path to Newest MSVC Compiler: ${NewestCompilerPath}")

    if (EXISTS ${NewestCompilerPath})
        #message("Compiler path: ${CompilerPath}")
        set(QT_PATH ${NewestCompilerPath})
        SET(QT_MISSING False)
    else()
        message("No QT5 installation found")
    endif()


ENDIF()

# use Qt_DIR approach so you can find Qt after cmake has been invoked
IF(NOT QT_MISSING)
    MESSAGE("-- Qt found: ${QT_PATH}")
	
    
    SET(Qt5_DIR "${QT_PATH}/lib/cmake/Qt5")
    SET(Qt5Widgets_DIR  "${QT_PATH}/lib/cmake/Qt5Widgets")
    SET(Qt5Test_DIR "${QT_PATH}/lib/cmake/Qt5Test")
	set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake")

    MESSAGE("Qt5Config.cmake path:  ${Qt5_DIR}")
ENDIF()