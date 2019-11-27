#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <chrono>

using namespace std;

namespace {
	const chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point endFrameTime = startTime;
}

namespace time {
	void markFrame() {
		lastFrameTime = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() -
		 endFrameTime).count() / 1e6f;
		endFrameTime = chrono::high_resolution_clock::now();
	}

	float dTime() { return lastFrameTime; }
	float accumTime() { 
		return 	chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() -
		 startTime).count() / 1e6f; 
	}
}

int main(){
	cout<<"Hello world"<<endl; 
	
	if (!glfwInit()){
		cout<<"ERROOOOOR"<<endl;
		return 0; 
	}
		 
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, APP_GL_MAJOR_VERSION);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, APP_GL_MINOR_VERSION);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  //make window
  GLFWwindow* window = glfwCreateWindow(800, 800, "scissorator", nullptr, nullptr); 
  if(window == NULL){
  	glfwTerminate();
  	cout<<"ERROOOOOR2"<<endl;
		return 0; 
  }
  
  while(!glfwWindowShouldClose(window)){
  	//mark the frame
  	time::markFrame(); 
  	
  	//input processing
  	
  	//
  }
}


















