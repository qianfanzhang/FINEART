CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-deprecated-copy -Wno-sign-compare")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

ADD_SUBDIRECTORY(deps/vecmath)

SET(RT_SOURCES
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/path_tracing.cpp
        src/scene_parser.cpp)

SET(RT_INCLUDES
        include/camera.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/material.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/plane.hpp
        include/path_tracing.hpp
        include/random.hpp
        include/ray.hpp
        include/scene_parser.hpp
        include/sphere.hpp
        include/utils.hpp
        include/texture.hpp
        include/transform.hpp
        include/triangle.hpp)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${RT_SOURCES} ${RT_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE vecmath)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

