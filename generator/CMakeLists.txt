project(generator)
add_executable(generator
	generator.h
	scene.h
	camera.h
	aquarium.h
	
	generator.cpp
	scene.cpp
	camera.cpp
	aquarium.cpp
	main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
