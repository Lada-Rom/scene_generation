project(generator)
add_executable(generator
	generator.h
	scene.h
	
	generator.cpp
	scene.cpp
	main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
