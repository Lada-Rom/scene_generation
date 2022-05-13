project(glut_test)
add_executable(glut_test glut_test.cpp)
target_link_libraries(glut_test ${OpenCV_LIBS} freeglut)
