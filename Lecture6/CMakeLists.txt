cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME Lecture6)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(GLEW_H /usr/local/include/GL)
set(GLFW_H /usr/local/include/GLFW)
set(GLAD_H /usr/local/include/glad)
set(SOIL_H /usr/local/include/SOIL)
set(KH_H /usr/local/include/KHR)
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H} ${SOIL_H} ${GLUT_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})


# 引用MacOS本身自带的OpenGL.framework
find_library(OPENGL OpenGL)
# 引用动态库
set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.dylib)
set(SOIL_LINK /usr/local/lib/libSOIL.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK} ${SOIL_LINK})


set(SOURCE_FILES main.cpp glad.c shader.h ${PROJECT_SOURCE_DIR}/glm/* )

add_executable(${PROJECT_NAME} main.cpp glad.c shader.h )

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework GLUT")
endif()