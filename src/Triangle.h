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

		//template <size_t WINDOW_HEIGHT, size_t WINDOW_WIDTH>
		

		/*template <typename colorVector, typename depthVector>
		void RenderCPU(colorVector& color, depthVector& depth);*/

		void setColor(std::vector<glm::vec3> traingleColor);

		float maxZ();

		float minZ();

		float min(float z0, float z1, float z2);

		float max(float z0, float z1, float z2);

		glm::vec3* Triangle::getVertex();


		template <size_t rows, size_t columns, size_t num_color>
		void RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], size_t WINDOW_HEIGHT, size_t WINDOW_WIDTH) {


			glm::mat4 viewport(

				(float)WINDOW_WIDTH / 2.0, 0.0f, 0.0f, 0.0f,

				0.0f, (float)WINDOW_HEIGHT / 2.0, 0.0f, 0.0f,

				0.0f, 0.0f, 1.0f, 0.0f,

				(float)WINDOW_WIDTH / 2.0, (float)WINDOW_HEIGHT / 2.0, 0.0f, 1.0f
			);


			//Object coordinate to clip space (NDC)

			//right multiply perspective and lookat matrix
			glm::mat4 top = glm::mat4(1.0); //identity

			top = top * viewport;
			top = top * projectionMatrix;
			top = top * modelViewMatrix;

			//homogenous coordinate
			glm::vec4 gl_positionV0(this->v[0], 1);
			glm::vec4 gl_positionV1(this->v[1], 1);
			glm::vec4 gl_positionV2(this->v[2], 1);

			gl_positionV0 = top * gl_positionV0;
			gl_positionV1 = top * gl_positionV1;
			gl_positionV2 = top * gl_positionV2;


			//convert back to vec3 by dividing by w coordinate
			glm::vec3 v0(gl_positionV0 / gl_positionV0.w);
			glm::vec3 v1(gl_positionV1 / gl_positionV1.w);
			glm::vec3 v2(gl_positionV2 / gl_positionV2.w);

			//maybe clamp later
			int _minX = ceil(min(v0.x, v1.x, v2.x));
			int _maxX = ceil(max(v0.x, v1.x, v2.x));
			int _minY = ceil(min(v0.y, v1.y, v2.y));
			int _maxY = ceil(max(v0.y, v1.y, v2.y));

			for (int i = _minY; i < _maxY; i++) { //height y
				for (int j = _minX; j < _maxX; j++) { //width x
					//perform the inside test
					for (int k = 0; k < 3; k++) {

						color[i][j][k] = 1;
					}
				}
			}
		}
};
