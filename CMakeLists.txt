cmake_minimum_required(VERSION 3.12)
project(project_name)

set(CMAKE_CXX_STANDARD 17)

# Add the spotify-json library
add_subdirectory(spotify-json)

# Include the spotify-json library headers
include_directories(spotify-json/include)

add_executable(project_name src/main.cpp)
target_link_libraries(project_name PRIVATE spotify-json)
