#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))


struct Baycentric {
	float alpha = -1;
	float beta = -1;
	float gamma = -1;
	Baycentric(float alpha, float beta, float gamma) : alpha(alpha), beta(beta), gamma(gamma) {};
	bool Inside() {
		if (this->alpha >= 0 && this->alpha <= 1) {
			if (this->beta >= 0 && this->beta <= 1) {
				if (this->gamma >= 0 && this->gamma <= 1) return true;
			}
		}
		return false;
	}

};



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

		float insideTest();

		Baycentric baycentricCoordinate(float xPos, float yPos, glm::vec3 A, glm::vec3 B, glm::vec3 C);

		void bufferUpdate();


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
			//compute alpha beta gamma and check if between 0 and 1

			//convert back to vec3 by dividing by w coordinate
			glm::vec3 v0(gl_positionV0 / gl_positionV0.w);
			glm::vec3 v1(gl_positionV1 / gl_positionV1.w);
			glm::vec3 v2(gl_positionV2 / gl_positionV2.w);

			//maybe clamp later
			int _minX = CLAMP(trunc(min(v0.x, v1.x, v2.x)), 0, WINDOW_WIDTH);
			int _maxX = CLAMP(ceil(max(v0.x, v1.x, v2.x)), 0, WINDOW_WIDTH);
			int _minY = CLAMP(trunc(min(v0.y, v1.y, v2.y)), 0, WINDOW_HEIGHT);
			int _maxY = CLAMP(ceil(max(v0.y, v1.y, v2.y)), 0, WINDOW_HEIGHT);
			float _minz = min(v0.z, v1.z, v2.z);

			for (int i = _minY; i < _maxY; i++) { //height y
				for (int j = _minX; j < _maxX; j++) { //width x
					//perform the inside test
					
					//center of pixel
					float xCenter = j + 0.5;
					float yCenter = i + 0.5;
					//calculate alpha beta and gamma
					Baycentric pos = baycentricCoordinate(xCenter, yCenter, v0, v1, v2);
					if (pos.Inside()) {

						//float maxDepth = pos.alpha * this->v[0].z + pos.beta * this->v[1].z + pos.gamma * this->v[2].z;
						float zInterpolate = v0.z * (1 - pos.gamma) + v1.z * pos.gamma;

						if (zInterpolate <= depth[i][j]) {
							color[i][j][0] = this->c[0].x * pos.alpha + this->c[1].x * pos.beta + this->c[2].x * pos.gamma;
							color[i][j][1] = this->c[0].y * pos.alpha + this->c[1].y * pos.beta + this->c[2].y * pos.gamma;
							color[i][j][2] = this->c[0].z * pos.alpha + this->c[1].z * pos.beta + this->c[2].z * pos.gamma;
							depth[i][j] = zInterpolate;
						}
						
					}

				}
			}
		}
};
