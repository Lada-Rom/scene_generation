project(generator)
add_executable(generator
	generator.h
	scene.h
	camera.h
	
	generator.cpp
	scene.cpp
	camera.cpp
	main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
