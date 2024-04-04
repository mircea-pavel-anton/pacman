cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project("pacman" VERSION 1.0 HOMEPAGE_URL "https://github.com/mirceanton/pacman")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
if(WIN32)
  set(SFML_STATIC_LIBRARIES TRUE)
endif()

# TODO edit me ON WINDOWS
SET(SFML_HOME "C:/SFML/SFML-2.6.1") 

file(GLOB_RECURSE GAME_SRC "src/*.cpp" "src/*.hpp" "src/*.mm")
set(SFML_DIR ${SFML_HOME}/lib/cmake/SFML)

find_package(SFML 2.5 REQUIRED COMPONENTS "graphics" "window" "audio" "system")
find_package(OpenGL REQUIRED) 

if(WIN32)
  add_executable(pacman WIN32 ${GAME_SRC} ${CMAKE_SOURCE_DIR}/res/win-icon.rc)
  target_link_libraries(pacman PRIVATE sfml-main)
elseif(APPLE)
  include(${CMAKE_SOURCE_DIR}/apple_cmake/MacBundle.cmake) 
  find_library(FOUNDATION_FRAMEWORK Foundation)
  target_link_libraries(pacman PRIVATE ${FOUNDATION_FRAMEWORK})
else()
  add_executable(pacman ${GAME_SRC})
endif()

# link all required libraries
target_link_libraries(pacman PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)

# include SFML headers
include_directories(${SFML_HOME}/include)

# show warnings (depending on C++ compiler)
if(MSVC)
  target_compile_options(pacman PRIVATE /W4)
else() 
  target_compile_options(pacman PRIVATE -Wall) 
endif()

# COPY resources folder to destination (WINDOWS, LINUX)
if(NOT APPLE)
  add_custom_command(
    TARGET pacman
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res
            $<TARGET_FILE_DIR:pacman>/res)
endif()

# COPY the audio DLL to destination (Windows only)
if(WIN32)
  add_custom_command(
    TARGET pacman
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_HOME}/bin/openal32.dll
            $<TARGET_FILE_DIR:pacman>/openal32.dll)
endif()
