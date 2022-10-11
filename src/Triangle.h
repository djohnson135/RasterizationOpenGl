#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>


class Triangle {
	private:
		glm::vec3 v[3];		// Triangle vertices
		glm::vec3 c[3];		// Vertex color
		glm::vec2 t[3];		// Texture coordinates

	public:

		// Default constructor
		Triangle();

		// Constructor without texture coordinates
		Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2);

		// Constructor with texture coordinates
		Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2);

		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool textureMode);

		// Rendering the triangle using CPU
		/*template <int WINDOW_HEIGHT, int WINDOW_WIDTH>
		void RenderCPU(float (& color)[WINDOW_HEIGHT][WINDOW_WIDTH][3], float (& depth)[WINDOW_HEIGHT][WINDOW_WIDTH]);*/

		
		void RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, float * color, float * depth, size_t WINDOW_HEIGHT, size_t WINDOW_WIDTH);

		/*template <typename colorVector, typename depthVector>
		void RenderCPU(colorVector& color, depthVector& depth);*/

		void setColor(std::vector<glm::vec3> traingleColor);

		float maxZ();

		float maxX();

		float maxY();

		float minZ();

		float minX();

		float minY();

		float min(float z0, float z1, float z2);

		float max(float z0, float z1, float z2);

		glm::vec3* Triangle::getVertex();
};
