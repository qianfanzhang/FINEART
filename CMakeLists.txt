CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(BINART CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-deprecated-copy -Wno-sign-compare")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

ADD_SUBDIRECTORY(deps/vecmath)

SET(BINART_SOURCES
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/kd_tree.cpp
        src/path_tracer.cpp
        src/scene.cpp)

SET(BINART_INCLUDES
        include/group/group.hpp
        include/group/basic_group.hpp
        include/group/kd_tree.hpp
        include/object/mesh.hpp
        include/object/object3d.hpp
        include/object/plane.hpp
        include/object/sphere.hpp
        include/camera.hpp
        include/bounding_box.hpp
        include/hit.hpp
        include/image.hpp
        include/material.hpp
        include/path_tracer.hpp
        include/random.hpp
        include/ray.hpp
        include/scene.hpp
        include/utils.hpp
        include/texture.hpp)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${BINART_SOURCES} ${BINART_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE vecmath)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
