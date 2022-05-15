project(generator)
add_executable(generator
	generator.h
	scene.h
	camera.h
	aquarium.h
	utils.h
	
	generator.cpp
	scene.cpp
	camera.cpp
	aquarium.cpp
	utils.cpp
	main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
