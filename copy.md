see below 

```C
# Define the source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")

# Define the destination directory inside the macOS bundle
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}.app/Contents/Resources")

# Function to copy files recursively
function(copy_resources SOURCE DEST)
    file(GLOB_RECURSE FILES "${SOURCE}/*")
    foreach(FILE ${FILES})
        file(RELATIVE_PATH REL_FILE ${SOURCE} ${FILE})
        get_filename_component(DIR ${REL_FILE} DIRECTORY)
        set(DEST_FILE "${DEST}/${REL_FILE}")
        if(DIR)
            file(MAKE_DIRECTORY "${DEST}/${DIR}")
        endif()
        add_custom_command(
            OUTPUT "${DEST_FILE}"
            COMMAND ${CMAKE_COMMAND} -E copy "${FILE}" "${DEST_FILE}"
            DEPENDS "${FILE}"
        )
    endforeach()
endfunction()

# Call the function to copy resources
copy_resources(${SOURCE_DIR} ${DESTINATION_DIR})
```