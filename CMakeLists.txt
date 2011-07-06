project(renderer)
cmake_minimum_required(VERSION 2.8)

if(APPLE)
find_library(GL_LIBRARIES opengl HINTS /System/Library)
find_library(GLUT_LIBRARIES glut)
else()
set(GL_LIBRARIES "")
set(GLUT_LIBRARIES glut)
endif()

add_executable(renderer main.cpp display.cpp perf_tracer.cpp)

target_link_libraries(renderer ${GL_LIBRARIES} ${GLUT_LIBRARIES})
