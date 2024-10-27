#
# This file is responsible for loading all dependencies to the project
#

include(FetchContent)
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${RELATIVE_BUILD_FOLDER}")

# Set up directory and find all .cmake files
set(dependency_dir "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
file(GLOB children RELATIVE ${dependency_dir} "${dependency_dir}/*.cmake")

# Remove "order.cmake" from the list if it exists
set(order_file "order.cmake")
list(REMOVE_ITEM children "${order_file}")

# Check for "order.cmake" and read the order if it exists
set(order_file_path "${dependency_dir}/${order_file}")
if(EXISTS "${order_file_path}")
    # Include the "order.cmake" to read the dependency order list
    include(${order_file_path})

    set(dependency_order )
    getOrder(dependency_order)
    # `getOrder()` should be defined in `order.cmake`
    # Move files that are in the order list to the beginning, in specified order
    set(ordered_children "")
    foreach(dep_name ${dependency_order})
        list(FIND children "${dep_name}" dep_index)
        if(dep_index GREATER -1)
            list(APPEND ordered_children "${dep_name}")
            list(REMOVE_AT children ${dep_index})
        else()
            message(WARNING "Dependency ${dep_name} in order.cmake not found in ${dependency_dir}")
        endif()
    endforeach()
    
    # Append remaining children that were not listed in `order.cmake`
    list(APPEND ordered_children ${children})
    set(children ${ordered_children})
endif()

# Print the dependency order
message("Dependency order:")
foreach(child ${children})
	message("  ${child}")
endforeach()

# Include each dependency file
foreach(child ${children})
    message("Loading dependency ${child}")
    include(${dependency_dir}/${child})
endforeach()
