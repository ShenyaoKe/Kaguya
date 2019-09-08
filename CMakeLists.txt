cmake_minimum_required(VERSION 3.12)

project(Kaguya)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-class-memaccess ")
endif()

include_directories(include src)
file(GLOB_RECURSE CORE_SOURCES src/*.cpp)
add_library(KaguyaCore STATIC ${CORE_SOURCES})

find_library (
        EMBREE_LIB
        NAMES embree3 libembree3 # what to look for
        HINTS "${CMAKE_SOURCE_DIR}/lib/x64" # where to look
        NO_DEFAULT_PATH # do not search system default paths
)
link_directories(${CMAKE_SOURCE_DIR}/lib/x64)

target_link_libraries(KaguyaCore ${EMBREE_LIB})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # since gcc, link with -lstdc++fs.
    target_link_libraries(KaguyaCore stdc++fs)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #target_link_libraries(KaguyaCore c++fs)
    #message("Compiler ID ${CMAKE_CXX_COMPILER_ID}")
endif()

add_subdirectory(example)
#add_subdirectory(viewer)
add_subdirectory(viewer_qt)