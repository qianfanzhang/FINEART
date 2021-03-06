cmake_minimum_required(VERSION 3.1)
project(FINEART CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-deprecated-copy -Wno-sign-compare -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_subdirectory(deps/vecmath)

set(FINEART_SOURCES
        src/main.cpp
        src/mesh.cpp
        src/kd_tree.cpp
        src/path_tracer.cpp
        src/scene.cpp
        src/utils.cpp)

set(MERGE_SOURCES
        src/merge.cpp
        src/utils.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} ${FINEART_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE vecmath)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_executable(merge ${MERGE_SOURCES})
target_link_libraries(merge PRIVATE vecmath)
target_include_directories(merge PRIVATE include)

find_package(OpenMP REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)

find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PNG_LIBRARY})
target_link_libraries(merge PUBLIC ${PNG_LIBRARY})