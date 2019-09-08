set(CMAKE_CXX_STANDARD 17)
set(EXAMPLE_FILES main.cpp)
add_executable(KaguyaExample ${EXAMPLE_FILES})

link_directories(lib/x64 bin/x64/Release)
target_link_libraries(KaguyaExample KaguyaCore)