cmake_minimum_required(VERSION 3.15)
project(JankyJunk LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(raylib)

add_executable(JankyJunk src/main.cpp)
target_link_libraries(JankyJunk PRIVATE raylib)
target_link_options(JankyJunk PRIVATE -static-libstdc++ -static-libgcc)
