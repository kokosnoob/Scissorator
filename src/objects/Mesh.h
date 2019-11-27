#ifndef _H_MESH
#define _H_MESH

#include <vector>

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

enum Primitive {
	TRIANGLE,
	QUAD,
	FULL_SCREEN_QUAD // For Post Processing.
};

struct Vertex {
	vec3 pos;
	vec3 col;
	vec3 nrm;
	vec2 tex;
	
	Vertex() : 
		pos(vec3(0.0)),
		col(vec3(0.0)),
		nrm(vec3(0.0)),
		tex(vec2(0.0))
	{ }
	
	Vertex(
		vec3 p_pos,
		vec3 p_col,
		vec3 p_nrm,
		vec2 p_tex
	) : Vertex() {
		pos = p_pos;
		col = p_col;
		nrm = p_nrm;
		tex = tex;
	}
	
	Vertex operator*(const mat4 &transMat) const {
		return Vertex(vec3(transMat * vec4(pos, 1.0)), col, nrm, tex);
	}
};

class Mesh {
	public :
		Mesh();
		Mesh(
			const vector<Vertex>,
			const vector<unsigned int>
		);
	
		Mesh(Primitive prim, mat4 transform);
		
		~Mesh();
		
		void bind();
		void unbind();
		
		void draw();
	
	private :
		// OpenGL Handles
		unsigned int id_VBO; // VBO holds vertices.
		unsigned int id_EBO; // EBO holds vertex indices.
		unsigned int id_VAO; // VAO refers to the above.
		
		// Shader Location Access Points
		const unsigned int loc_vertPos = 0; // Position @ 0
		const unsigned int loc_vertCol = 1; // Color @ 1
		const unsigned int loc_vertNrm = 2; // Normal @ 2
		const unsigned int loc_vertTex = 3; // Texture Coords @ 3
		
		// Actual Data Fields
		vector<Vertex> vertices;
		vector<unsigned int> indices;
};

#endif
