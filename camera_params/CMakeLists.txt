project(camera_params)
add_executable(camera_params aquarium.h aquarium.cpp camera_params.cpp camera_params.h scene.cpp scene.h)
target_link_libraries(camera_params ${OpenCV_LIBS} freeglut)
