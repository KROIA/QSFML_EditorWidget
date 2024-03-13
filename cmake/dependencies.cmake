cmake_minimum_required(VERSION 3.1.0)

# # Path where the dependencies are located after download
# set(dependenciesBasePath dependencies)
# 
# # List of dependencies
# # Each dependency is a cmake file
# set(deps dep_easy_profiler)
# 
# set(sharedDeps )
# set(staticDeps )
# set(staticProfileDeps dep_easy_profiler)
# 
# foreach(dep ${deps})
#   include(${dependenciesBasePath}/${dep}.cmake)
# endforeach()
# 
# function(loadDependencies sharedDepsOut staticDepsOut staticProfileDepsOut)
#   foreach(dep ${sharedDeps})
# 	load_${dep}(sharedDepsOut)
#   endforeach()
# endfunction()



#
#
#

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dependencies/cache")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")


cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

# Add each subdirectory containing library.cmake files
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*.cmake)
foreach(child ${children})
    message("Including ${child}")
    include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/${child})
endforeach()