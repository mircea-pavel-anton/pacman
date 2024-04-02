# icon
set(MACOSX_BUNDLE_ICON_FILE "macos_icon.icns")
set(application_icon "${CMAKE_SOURCE_DIR}/res/${MACOSX_BUNDLE_ICON_FILE}")
set_source_files_properties(${application_icon}
                            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

# images
# file(GLOB_RECURSE my_images "${CMAKE_SOURCE_DIR}/res/*")
# foreach(FILE ${my_images})
# #  message(STATUS ${FILE})
#   get_filename_component(FILENAME ${FILE} NAME)
#   if(NOT FILENAME STREQUAL ".DS_Store")
#     # skip .DS_STORE
#     file(RELATIVE_PATH NEW_FILE "${CMAKE_SOURCE_DIR}/" ${FILE})
#     get_filename_component(REL_DIR ${NEW_FILE} DIRECTORY)
#     get_filename_component(END_FILE ${NEW_FILE} NAME)
#      if(DIR)
#             file(MAKE_DIRECTORY "${DEST}/${DIR}")
#         endif()
#      message(STATUS "RELATIVE DIR ${REL_DIR} END_FILE ${END_FILE}" )
#     set_source_files_properties(${FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${REL_DIR}/${END_NAME}")
#   endif()
# endforeach()


add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
               ${GAME_SRC} ${application_icon} ${CMAKE_SOURCE_DIR}/res/)

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
           RESOURCE ${CMAKE_SOURCE_DIR}/res/ )
