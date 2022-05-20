project(generator)
add_executable(generator
	generator.h
	scene.h
	camera.h
	aquarium.h
	daphnia.h
	texture.h
	utils.h
	
	generator.cpp
	scene.cpp
	camera.cpp
	aquarium.cpp
	daphnia.cpp
	texture.cpp
	utils.cpp
	main.cpp)
target_link_libraries(generator ${OpenCV_LIBS} freeglut)
