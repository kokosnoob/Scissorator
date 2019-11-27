#include "camera.h"
#include <iostream>

// OpenGL Libraries
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Constructors
Camera::Camera() : 
	
	fov(0.0),
	clipNear(0.0),
	clipFar(0.0),
	
	pos(vec3(0.0, 0.0, 0.0)),
	
	moveSpeed(1.0f),
	
	right(vec3(0.0, 0.0, 0.0)),
	up(vec3(0.0, 0.0, 0.0)),
	front(vec3(0.0, 0.0, 0.0))

{ }

Camera::Camera(vec3 par_pos): Camera() {
	fov = 80.0f;
	clipNear = 0.1f;
	clipFar = 100.0f;
	
	moveSpeed = 1.0f;
	
	pos = par_pos;
	front = vec3(0.0f, 0.0f, -1.0f);
}

Camera::~Camera() { }

glm::mat4 Camera::getView() {
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::getProj(float aspect) {
	return glm::perspective(glm::radians(80.0f / 2.0f), aspect, clipNear, clipFar);
}

//Input Methods
void Camera::keyInput(const vector<int> keysHeld, const float dTime) {
	float vel = moveSpeed * dTime;
	
	//If shift is one of the (held) keys, then speedup!
	if (find(keysHeld.begin(), keysHeld.end(), GLFW_KEY_LEFT_SHIFT) != keysHeld.end()) {
		vel *= 2.5;
	}
	
	//We assume that, if this function is called, then the key is pressed.
	for (const int glfwKey : keysHeld) {
		
		if (glfwKey == GLFW_KEY_W) pos += front * vel;
		if (glfwKey == GLFW_KEY_A) pos -= right * vel;
		if (glfwKey == GLFW_KEY_S) pos -= front * vel;
		if (glfwKey == GLFW_KEY_D) pos += right * vel;
	}

}






















