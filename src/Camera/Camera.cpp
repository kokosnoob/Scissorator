#include "Camera.h"

// System Libraries
#include <algorithm>



// OpenGL Libraries
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>



// Headers
#include "program/PContext.h"



// Constructors
Camera::Camera() : 
	camModel(CameraModel::FLY_AROUND),
	
	fov(0.0),
	clipNear(0.0),
	clipFar(0.0),
	
	pos(vec3(0.0, 0.0, 0.0)),
	
	moveSpeed(0.0),
	rotSen(0.0),
	
	right(vec3(0.0, 0.0, 0.0)),
	up(vec3(0.0, 0.0, 0.0)),
	front(vec3(0.0, 0.0, 0.0)),
	
	eulerOrient(vec3(0.0, 0.0, 0.0)),
	
	worldUp(vec3(1.0))
{ }

Camera::Camera(	vec3 par_pos,
				float pitch,
				float yaw,
				CameraModel par_camModel
) : Camera() {
	camModel = par_camModel;
	
	fov = 80.0f;
	clipNear = 0.1f;
	clipFar = 100.0f;
	
	moveSpeed = 1.0f;
	rotSen = 0.1f;
	
	worldUp = vec3(0.0f, 1.0f, 0.0f);
	
	pos = par_pos;
	front = vec3(0.0f, 0.0f, -1.0f);
	setEuler(pitch, yaw, 0); //Sets front vector
}

Camera::Camera(vec3 par_pos) : Camera(par_pos, 0.0f, 90.0f) { }

Camera::~Camera() { }



//Methods
glm::mat4 Camera::getView() {
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::getProj(float aspect) {
	return glm::perspective(glm::radians(80.0f / 2.0f), aspect, clipNear, clipFar);
}

void Camera::addEuler(const float pitch, const float yaw, const float roll) {
	setEuler(eulerOrient + glm::vec3(pitch, yaw, roll)); //Add the euler vectors to accumulate this euler rotation.
}

void Camera::addEuler(const vec3 eulerRot) {
	addEuler(eulerRot[0], eulerRot[1], eulerRot[2]);
}

void Camera::setEuler(const float pitch, const float yaw, const float roll) {
	//TODO: Make roll work by adjusting up vector.
	
	using namespace glm;
	
	//Make sure the screen never flips when pitch is out of bounds.
	float adj_pitch = pitch;
	
	if (pitch > 89.0f) adj_pitch = 89.0f;
	if (pitch < -89.0f) adj_pitch = -89.0f;
	
	setFrontVec(mkFrontVec(adj_pitch, yaw));
	
	eulerOrient = vec3(adj_pitch, yaw, roll);
}

void Camera::setEuler(const vec3 eulerRot) {
	setEuler(eulerRot[0], eulerRot[1], eulerRot[2]);
}



//Helpers
glm::vec3 Camera::mkFrontVec(const float pitch, const float yaw) {
	using namespace glm;
	
	vec3 acc_front = glm::vec3(0.0f, 0.0f, 0.0f);
	
	acc_front.x = cos(radians(yaw)) * cos(radians(pitch));
	acc_front.y = sin(radians(pitch));
	acc_front.z = sin(radians(yaw)) * cos(radians(pitch));
	
	return acc_front;
}

void Camera::setFrontVec(const vec3 frontVec) { //FUTURE: THis is where roll would go.
	front = glm::normalize(frontVec);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}



//Input Methods
void Camera::keyInput(const vector<int> keysHeld, const float dTime) {
	float vel = moveSpeed * dTime;
	
	//If shift is one of the (held) keys, then speedup!
	if (std::find(keysHeld.begin(), keysHeld.end(), GLFW_KEY_LEFT_SHIFT) != keysHeld.end()) {
		vel *= 2.5;
	}
	
	//We assume that, if this function is called, then the key is pressed.
	if (camModel == CameraModel::FLY_AROUND) {
		for (const int glfwKey : keysHeld) {
			
			if (glfwKey == GLFW_KEY_W) pos += front * vel;
			if (glfwKey == GLFW_KEY_A) pos -= right * vel;
			if (glfwKey == GLFW_KEY_S) pos -= front * vel;
			if (glfwKey == GLFW_KEY_D) pos += right * vel;
			if (glfwKey == GLFW_KEY_Q) pos -= worldUp * vel;
			if (glfwKey == GLFW_KEY_E) pos += worldUp * vel;
			
		}
	}
}

void Camera::mouseInput(const double dX, const double dY) {
	//~ std::cout << dX * rotSen << " " << dY * rotSen << std::endl;
	
	addEuler(-dY * rotSen, dX * rotSen, 0);
}

void Camera::scrollInput(const double dY) {	
	moveSpeed += dY * 0.25;
}
