cmake_minimum_required(VERSION 3.21.0)
project(BorisPNCCommon VERSION 0.0.1)
SET(THIS_VERSION 0.0.1)

# Top Level Check
if( BorisPNCCommon_IS_TOP_LEVEL)
    SET(IS_TOP_DIR YES)
    SET(ENABLE_TEST YES)
else()
    SET(IS_TOP_DIR NO)
    SET(ENABLE_TEST NO)
endif()

# Build Config
if(IS_TOP_DIR)
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
    SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
endif()

# <<<<< Find Dependencies <<<<< #
if(NOT BORIS_MATH_FOUND)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/SecondParty)
        execute_process(COMMAND mkdir ${CMAKE_CURRENT_SOURCE_DIR}/SecondParty)
    endif()
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/SecondParty/BorisMathLib)
        execute_process(COMMAND git clone git@github.com:BorisVandermeer/BorisMathLib.git --depth=1
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SecondParty)
    endif()

    add_subdirectory(SecondParty/BorisMathLib)
endif()
# >>>>> Find Dependencies >>>>> #


# Find SRCs

SET(BORIS_PNC_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(Trajectory SRC_FILES_P1)
aux_source_directory(Models SRC_FILES_P2)

set(SRC_FILES ${SRC_FILES_P1} ${SRC_FILES_P2})


add_library(BORIS_PNC_COMMON_LIB ${SRC_FILES})
target_include_directories(BORIS_PNC_COMMON_LIB PUBLIC ${BORIS_MATH_INCLUDE_DIRS} ${BORIS_PNC_COMMON_INCLUDE_DIRS})

if(IS_TOP_DIR)
    SET(BORIS_PNC_COMMON_VERSION THIS_VERSION)
    SET(BORIS_PNC_COMMON_FOUND YES)
    SET(BORIS_PNC_COMMON_LIBRARIES BORIS_PNC_COMMON_LIB)
    SET(BORIS_PNC_COMMON_INCLUDE_DIRS ${BORIS_PNC_COMMON_INCLUDE_DIRS})
else()
    SET(BORIS_PNC_COMMON_VERSION THIS_VERSION PARENT_SCOPE)
    SET(BORIS_PNC_COMMON_FOUND YES PARENT_SCOPE)
    SET(BORIS_PNC_COMMON_LIBRARIES BORIS_PNC_COMMON_LIB PARENT_SCOPE)
    SET(BORIS_PNC_COMMON_INCLUDE_DIRS ${BORIS_PNC_COMMON_INCLUDE_DIRS} PARENT_SCOPE)
endif()

if(ENABLE_TEST)
    add_subdirectory(Test)
endif()