cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(epidemic)

set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG)
endif()

# Warning flags
if(MSVC)
    # warning level 3 and all warnings as errors
#    add_compile_options(/W3 /WX)
    add_compile_options(/W3)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)

# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.2
)

# Adds Catch2 testing library
FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_library(catch2 INTERFACE)
    target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif()

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

list(APPEND CORE_SOURCE_FILES   src/configuration.cc
                                src/individual.cc
                                src/utils.cc
                                src/route.cc
                                src/location.cc
                                src/city.cc
                                src/hand.cc
                                src/isolation.cc
                                src/view.cc
                                src/scene_manager.cc)

list(APPEND SOURCE_FILES    ${CORE_SOURCE_FILES}
                            src/scenes/menu.cc
                            src/scenes/game.cc
                            src/scenes/end.cc
                            src/epidemic_app.cc)

list(APPEND TEST_FILES  tests/individual_test.cc
                        tests/configuration_test.cc
                        tests/utils_test.cc
                        tests/route_test.cc
                        tests/city_test.cc
                        tests/hand_test.cc
                        tests/isolation_test.cc
                        tests/view_test.cc
                        tests/scene_manager_test.cc
                        tests/scenes_test/game_test.cc)

ci_make_app(
        APP_NAME        epidemic
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         apps/epidemic.cc ${SOURCE_FILES}
        INCLUDES        include
        BLOCKS          Cinder-ImGui
)

ci_make_app(
        APP_NAME        epidemic-test
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         tests/test_main.cc ${SOURCE_FILES} ${TEST_FILES}
        INCLUDES        include
        LIBRARIES       catch2
        BLOCKS          Cinder-ImGui
)

if(MSVC)
    set_property(TARGET epidemic-test APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:CONSOLE")
endif()