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



# Deploys the QT dependencies for a given exe file to a specific output path

# Function name: DEPLOY_QT
# Params: QT_PATH           Path to the QT installation. C:\Qt\5.15.2\msvc2015_64
#         targetExePath     Path to the exe file: "$<TARGET_FILE_DIR:profiler_gui>/$<TARGET_FILE_NAME:profiler_gui>"
#         outputPath        Path in which the dependencies will be saved to project/bin


# Initialize a counter if it doesn't exist
if (NOT DEFINED TARGET_COUNTER)
    set(TARGET_COUNTER 0 CACHE INTERNAL "Target counter")
endif()

function(DEPLOY_QT QT_PATH targetExePath outputPath)
    set(DEPLOY_COMMAND  "${QT_PATH}/bin/windeployqt.exe"
                --no-compiler-runtime
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                --no-angle
                --no-webkit2
                --pdb)


   string(RANDOM LENGTH 20 UNIQUE_TARGET_NAME)
   message("DummyTargetName: ${UNIQUE_TARGET_NAME}")

   add_custom_target(${UNIQUE_TARGET_NAME} ALL
       DEPENDS "${targetExePath}"
   )
    # Deploy easy_profiler gui to bin folder
     add_custom_command(TARGET ${UNIQUE_TARGET_NAME}
         COMMAND ${DEPLOY_COMMAND} 
             --dir "${outputPath}"
            "${targetExePath}"
 	    COMMENT "Running windeployqt..." "${QT_PATH}/bin/windeployqt.exe" "${outputPath}"
        DEPENDS UNIQUE_TARGET_NAME
     )
 endfunction()


function(copyLibraryHeaders headerRootFolder destinationPath destinationFolderName)
     # Copy the folder
    #message("COPY ${headerRootFolder} DESTINATION ${CMAKE_BINARY_DIR}")
    file(COPY ${headerRootFolder}
         DESTINATION ${CMAKE_BINARY_DIR})

    
    get_filename_component(FOLDER_NAME ${headerRootFolder} NAME)
    #message("FOLDER_NAME ${FOLDER_NAME}")


    #message("REMOVE_RECURSE ${CMAKE_BINARY_DIR}/${destinationFolderName}")
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/${destinationFolderName}")



    #message("RENAME ${CMAKE_BINARY_DIR}/${FOLDER_NAME}
    #            ${CMAKE_BINARY_DIR}/${destinationFolderName}")

    # Rename the copied folder
    file(RENAME ${CMAKE_BINARY_DIR}/${FOLDER_NAME}
                ${CMAKE_BINARY_DIR}/${destinationFolderName})

    #message("DIRECTORY ${CMAKE_BINARY_DIR}/${destinationFolderName}
    #        DESTINATION ${destinationPath}")
    # Install the modified folder
    install(DIRECTORY ${CMAKE_BINARY_DIR}/${destinationFolderName}
            DESTINATION ${destinationPath})

    message("Installing headers from: ${headerRootFolder} to ${destinationPath}/${destinationFolderName}")

endfunction()
