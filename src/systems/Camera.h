#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_

#include <glm/glm.hpp>

class Camera {
private:
	void updateVectors();

public:
	Camera(glm::vec3 position, float fov);

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 position;
	glm::mat4 rotation;

	float fov;

	void rotate(float x, float y, float z);

	glm::mat4 getProjection();
	glm::mat4 getView();

};

#endif