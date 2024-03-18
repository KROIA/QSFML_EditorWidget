
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")

# Add each subdirectory containing library.cmake files
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*.cmake)
foreach(child ${children})
    message("Including ${child}")
    include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/${child})
endforeach()