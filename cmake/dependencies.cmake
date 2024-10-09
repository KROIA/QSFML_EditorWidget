
include(FetchContent)
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")




# Add each subdirectory containing library.cmake files
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*.cmake)

# Move the easy_profiler.cmake file to the start of the list
# so that all other libraries can use it
# Define the file you want to move to the start
set(target_file "easy_profiler.cmake")
# Find the index of the target file in the list
list(FIND children "${target_file}" target_index)
# If the target file is found, move it to the start of the list
if(target_index GREATER -1)
    # Remove the target file from its current position
    list(REMOVE_AT children ${target_index})
    
    # Insert the target file at the beginning of the list
    list(INSERT children 0 "${target_file}")
    
    #message("Moved ${target_file} to the start of the list.")
endif()

foreach(child ${children})
    message("Including ${child}")
    include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/${child})
endforeach()