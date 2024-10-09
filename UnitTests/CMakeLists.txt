## 
## This file will include all subdirectories in the current directory
## Each subdirectory should contain a CMakeLists.txt file
## 
##


## USER_SECTION_START 1

## USER_SECTION_END

# Get a list of all subdirectories in the current directory
file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

## USER_SECTION_START 2

## USER_SECTION_END

## This is the unit test directory, the library UnitTest is included here
include(UnitTest.cmake)

## USER_SECTION_START 3

## USER_SECTION_END

# Loop over each subdirectory and add it as a subdirectory in the project
foreach(subdirectory ${subdirectories})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory})
## USER_SECTION_START 4

## USER_SECTION_END
        add_subdirectory(${subdirectory})
## USER_SECTION_START 5

## USER_SECTION_END
    endif()
## USER_SECTION_START 6

## USER_SECTION_END
endforeach()

## USER_SECTION_START 7

## USER_SECTION_END