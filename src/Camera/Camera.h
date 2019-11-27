#ifndef _H_CAMERA
#define _H_CAMERA

// Definition Headers
#include "globals.h"

#include "data/CameraModel.h"



// Forward Declarations



class Camera {
	public:
		// Attributes
		CameraModel camModel; //Type of camera model we want to use.
		
		float fov; //Horizontal field of view.
		float clipNear; //Near clipping plane
		float clipFar; //Far clipping plane
		
		vec3 pos; //Position of the camera
		
		float moveSpeed; //Movement speed of camera, in units/sec.
		float rotSen; //Sensitivity
		
		// Constructors
		Camera();
		Camera(	vec3 par_pos,
				float pitch,
				float yaw,
				CameraModel camModel = CameraModel::FLY_AROUND
		);
		
		Camera(vec3 par_pos);
		
		~Camera();
		
		// Methods
		mat4 getView();
		mat4 getProj(float);
		
		void addEuler(const float, const float, const float);
		void addEuler(const vec3);
		
		void setEuler(const float, const float, const float); //Change the camera's vectors to correspond to these euler angles.
		void setEuler(const vec3); //Change the camera's vectors to correspond to these euler angles (x, y, z).
		
		// Input Methods
		void keyInput(const vector<int>, const float); //Updates Camera based on keyboard input parameters.
		void mouseInput(const double, const double); //Updates Camera based on mouse input parameters.
		void scrollInput(const double); //Updates Camera based on scrollwheel input parameters.
		
	private:
		// Attributes
		vec3 right; // Local +x axis of camera
		vec3 up; // Local +y axis of camera
		vec3 front; // Local +z axis of camera (note this is backwards in right-handed coordinate system).
		
		vec3 eulerOrient; //Pitch, Yaw, Roll
		
		vec3 worldUp; //Global +y axis.
		
		// Helper Methods
		vec3 mkFrontVec(const float, const float);
		void setFrontVec(const vec3);
};

#endif
