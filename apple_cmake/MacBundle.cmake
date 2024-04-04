# COLLECT all assets inside 'res/' folder into a list
list(APPEND ASSET_FILES "")

# start recursing the directory tree
file(GLOB_RECURSE my_images "${CMAKE_SOURCE_DIR}/res/*")
foreach(FILE ${my_images})
  get_filename_component(FILENAME ${FILE} NAME)
  if(NOT FILENAME STREQUAL ".DS_Store")
    # skip .DS_Store
    file(RELATIVE_PATH NEW_FILE "${CMAKE_SOURCE_DIR}/" ${FILE})
    get_filename_component(REL_DIR ${NEW_FILE} DIRECTORY) # parent dir
    get_filename_component(END_FILE ${NEW_FILE} NAME) # filename (end of path)
    list(APPEND ASSET_FILES "${REL_DIR}/${END_FILE}")
    set_source_files_properties(${REL_DIR}/${END_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION
                                        "Resources/${REL_DIR}")
  endif()
endforeach() 

# icon
set(application_icon "${CMAKE_SOURCE_DIR}/res/macos_icon.icns")
set_source_files_properties(${application_icon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

# build executable as MACOSX BUNDLE                           
add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
               ${GAME_SRC} ${application_icon} "${ASSET_FILES}") 

# Metadata for Apple info.plist 
set_target_properties(
  ${CMAKE_PROJECT_NAME}
  PROPERTIES BUNDLE TRUE
             XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
             XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
             XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS
             "@executable_path/../Frameworks"
             MACOSX_BUNDLE_BUNDLE_NAME "${CMAKE_PROJECT_NAME}"
             MACOSX_BUNDLE_GUI_IDENTIFIER "com.mirceanton.${CMAKE_PROJECT_NAME}"
             MACOSX_BUNDLE_COPYRIGHT "(c) 2022, mirceanton"
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
             RESOURCE ${ASSET_FILES}) 
