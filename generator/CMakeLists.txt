project(generator)
add_executable(generator main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
