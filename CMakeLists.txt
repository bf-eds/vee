cmake_minimum_required(VERSION 3.15)
project(veerouter)

set(CMAKE_CXX_STANDARD 17)

#add_compile_definitions(_GLIBCXX_DEBUG_PEDANTIC)
#add_compile_definitions(_GLIBCXX_DEBUG)

add_library(route SHARED lib/src/route.cpp lib/src/road_router.cpp)
target_include_directories(route PUBLIC ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
add_executable(veerouter main.cpp)
target_link_libraries(veerouter route)
