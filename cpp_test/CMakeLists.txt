project(cpp_test)
add_executable(cpp_test cpp_test.cpp)
target_link_libraries(cpp_test ${OpenCV_LIBS} freeglut)
