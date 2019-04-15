project(CG2019ICMC)

cmake_minimum_required(VERSION 3.10)

include(ExternalProject)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")
include( glfw )
include( glm )

find_package(OpenGL REQUIRED)
set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} CACHE STRING "")
set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES} CACHE STRING "")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glad" "external/glad")
set(GLAD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include")

set(CG2019ICMC_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp_src)
add_subdirectory(cpp_src)

##################################################################
# EXECUTAVEIS

add_executable( catavento main.cpp )
target_link_libraries( catavento glad ${GLFW_LIBRARIES} cg2019cpp )
target_include_directories( catavento PUBLIC 
                            ${CG2019ICMC_CPP_INCLUDE_DIR}
                            ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDES} )
