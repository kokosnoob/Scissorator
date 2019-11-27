#include "Mesh.h"


#include <iostream>
#include <vector>
#include <glm/glm.hpp>



#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>



using namespace std;

Mesh::Mesh() : 
	id_VBO(0),
	id_EBO(0),
	id_VAO(0),
	
	vertices(vector<Vertex>()),
	indices(vector<unsigned int>())
{ }



Mesh::Mesh(	const std::vector<Vertex> p_vertices,
			const std::vector<unsigned int> p_indices
) : Mesh() {
	vertices = p_vertices;
	indices = p_indices;
}

Mesh::Mesh(Primitive prim, mat4 transform) : Mesh() {
	switch (prim) {
		case (Primitive::TRIANGLE) :			
			vertices = {
				Vertex({0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.5f, 1.0f}) * transform, //Top
				Vertex({0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}) * transform, //Bottom Right
				Vertex({-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}) * transform, //Bottom Left
			};
			
			indices = {
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};
			
			break;
		
		case (Primitive::QUAD) :			
			vertices = {
				Vertex({0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}) * transform, //Top Right
				Vertex({0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}) * transform, //Bottom Right
				Vertex({-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}) * transform, //Bottom Left
				Vertex({-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}) * transform //Top Left
			};
			
			indices = {
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};
			
			break;
		
		case (Primitive::FULL_SCREEN_QUAD) :			
			vertices = {
				Vertex({1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}) * transform, //Top Right
				Vertex({1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}) * transform, //Bottom Right
				Vertex({-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}) * transform, //Bottom Left
				Vertex({-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}) * transform //Top Left
			};
			
			indices = {
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};
			
			break;
		
		default :
			cout << "Requested Primitive is not implemented!" << endl;
	}
}

Mesh::~Mesh() { }



void Mesh::bind() {
	// Create a Vertex Array Object
	glGenVertexArrays(1, &id_VAO);
	
	
	// Declare Buffers
	glGenBuffers(1, &id_VBO); // Vertex Buffer Object, for Vertices.
	glGenBuffers(1, &id_EBO); // Element Buffer Object, for Indices
	
	
	// Bind w/Particular Size (assume the mesh data will be static)
	glBindVertexArray(id_VAO); // Bind VAO while binding VBO/EBO
	
	glBindBuffer(GL_ARRAY_BUFFER, id_VBO); // Bind VBO Buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			vertices.size() * sizeof(Vertex) // Amount of VRAM Filled by Vertices
		),
		&vertices[0], // Pointer to Vertices; guaranteed contiguous.
		GL_STATIC_DRAW // Will be modified ONCE.
	);
	
	glBindBuffer(GL_ARRAY_BUFFER, id_EBO); // Bind EBO Buffer,
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			indices.size() * sizeof(unsigned int) // Amount of VRAM to Allocate
		),
		&indices[0], // Pointer to Indices; guaranteed contiguous.
		GL_STATIC_DRAW // Will be modified ONCE.
	);
	
	
	// Supply Vertex Attributes to Shader via Interlacing
	glVertexAttribPointer(
		loc_vertPos, // Shader Location to Bind To
		3, // "Components" per Data Point
		GL_FLOAT, // Type of "Component"
		GL_FALSE, // Don't "normalize" values on read.
		sizeof(Vertex), // "Stride": Distance between Data Points
		reinterpret_cast<void*>( offsetof(Vertex, pos) ) // "Start Point"
	);
	glEnableVertexAttribArray(loc_vertPos);
	
	glVertexAttribPointer(
		loc_vertCol, // Shader Location to Bind To
		3, // "Components" per Data Point
		GL_FLOAT, // Type of "Component"
		GL_FALSE, // Don't "normalize" values on read.
		sizeof(Vertex), // "Stride": Distance between Data Points
		reinterpret_cast<void*>( offsetof(Vertex, col) ) // "Start Point"
	);
	glEnableVertexAttribArray(loc_vertCol);
	
	glVertexAttribPointer(
		loc_vertNrm, // Shader Location to Bind To
		3, // "Components" per Data Point
		GL_FLOAT, // Type of "Component"
		GL_FALSE, // Don't "normalize" values on read.
		sizeof(Vertex), // "Stride": Distance between Data Points
		reinterpret_cast<void*>( offsetof(Vertex, nrm) ) // "Start Point"
	);
	glEnableVertexAttribArray(loc_vertCol);
	
	glVertexAttribPointer(
		loc_vertTex, // Shader Location to Bind To
		2, // "Components" per Data Point
		GL_FLOAT, // Type of "Component"
		GL_FALSE, // Don't "normalize" values on read.
		sizeof(Vertex), // "Stride": Distance between Data Points
		reinterpret_cast<void*>( offsetof(Vertex, tex) ) // "Start Point"
	);
	glEnableVertexAttribArray(loc_vertTex);
	
	
	// Cleanup Context
	glBindVertexArray(0);
}

void Mesh::unbind() {
	// Delete Resources
	glDeleteVertexArrays(1, &id_VAO);
	glDeleteBuffers(1, &id_VBO);
	glDeleteBuffers(1, &id_EBO);
}

void Mesh::draw() {
	// Mesh must be bind()'ed
	
	// Bind the VAO
	glBindVertexArray(id_VAO); // Binds the VBO/EBO automatically.
	
	// Draw Elements!
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	
	// Cleanup!
	glBindVertexArray(0);
}
