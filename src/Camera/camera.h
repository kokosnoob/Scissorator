#ifndef _H_CAMERA
#define _H_CAMERA

#include "glm/glm.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

class Camera{
	public:
	
		float fov; //Horizontal field of view.
		float clipNear; //Near clipping plane
		float clipFar; //Far clipping plane
		
		vec3 pos; //Position of the camera
		
		float moveSpeed; //Movement speed of camera, in units/sec.
		
		// Constructors
		Camera();		
		Camera(vec3 par_pos);
		
		~Camera();
		
		// Methods
		mat4 getView();
		mat4 getProj(float);
		
		// Input Methods
		void keyInput(const vector<int>, const float); //Updates Camera based on keyboard input parameters.
			private:
			vec3 right; // Local +x axis of camera
			vec3 up; // Local +y axis of camera
			vec3 front; // Local +z axis of camera (note this is backwards in right-handed coordinate system).
}

#endif
