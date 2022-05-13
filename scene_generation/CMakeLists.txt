project(scene_generation)
add_executable(scene_generation 
	aquarium.h 
	aquarium.cpp 
	daphnia.h 
	daphnia.cpp 
	scene.h 
	scene.cpp 
	scene_generation.cpp)
target_link_libraries(scene_generation ${OpenCV_LIBS} freeglut imgui)
