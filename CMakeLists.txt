project(renderer)
cmake_minimum_required(VERSION 2.8)

find_library(GL_LIBRARIES opengl HINTS /System/Library)
find_library(GLUT_LIBRARIES glut)

message(${GL_LIBRARIES})
message(${GLUT_LIBRARIES})

add_executable(renderer main.cpp display.cpp)

target_link_libraries(renderer ${GL_LIBRARIES} ${GLUT_LIBRARIES})
