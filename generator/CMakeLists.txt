project(generator)
add_executable(generator 
	generator.h 
	
	generator.cpp
	main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
