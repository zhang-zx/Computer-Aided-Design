cmake_minimum_required(VERSION 3.12)
project(Lecture4)

set(CMAKE_CXX_STANDARD 14)

set(GLEW_H /usr/local/include/GL)
set(GLFW_H /usr/local/include/GLFW)
set(GLAD_H /usr/local/include/glad)
set(KH_H /usr/local/include/KHR)
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H})
include_directories(${PROJECT_SOURCE_DIR})


# 引用MacOS本身自带的OpenGL.framework
find_library(OPENGL OpenGL)
# 引用动态库
set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})



set(SOURCE_FILES main.cpp glad.c shader.h ${PROJECT_SOURCE_DIR}/glm/*)

add_executable(Lecture4 main.cpp glad.c shader.h)

if (APPLE)
    target_link_libraries(Lecture4 "-framework OpenGL")
    target_link_libraries(Lecture4 "-framework GLUT")
endif()