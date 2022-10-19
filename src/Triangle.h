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

struct BoundingBox {
	int minY, maxY, minX, maxX = 0;

	BoundingBox(int minY, int maxY, int minX, int maxX) {
		this->minY = minY;
		this->maxY = maxY;
		this->minX = minX;
		this->maxX = maxX;
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

		void setColor(std::vector<glm::vec3> traingleColor);

		float maxZ();

		float minZ();

		float min(float z0, float z1, float z2);

		float max(float z0, float z1, float z2);

		glm::vec3* Triangle::getVertex();

		Baycentric baycentricCoordinate(float xPos, float yPos, glm::vec3 A, glm::vec3 B, glm::vec3 C);

		template <size_t rows, size_t columns, size_t num_color>
		void colorMapping(BoundingBox boundingBox, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
		{
			for (int i = boundingBox.minY; i < boundingBox.maxY; i++) { //height y
				for (int j = boundingBox.minX; j < boundingBox.maxX; j++) { //width x
					//perform the inside test

					//center of pixel
					float xCenter = j + 0.5;
					float yCenter = i + 0.5;
					//calculate alpha beta and gamma
					Baycentric pos = baycentricCoordinate(xCenter, yCenter, v0, v1, v2);
					if (pos.Inside()) {

						float zInterpolate = v0.z * pos.alpha + v1.z * pos.beta + v2.z * pos.gamma;

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
		template <size_t rows, size_t columns, size_t num_color>
		void textureNearest(BoundingBox boundingBox, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {

		}

		template <size_t rows, size_t columns, size_t num_color>
		void textureLinear(BoundingBox boundingBox, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {

		}

		template <size_t rows, size_t columns, size_t num_color>
		void textureMipMap(BoundingBox boundingBox, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {

		}

		template <size_t rows, size_t columns, size_t num_color>
		void textureMapping(BoundingBox boundingBox, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int textureMode)
		{
			switch (textureMode) {
			case  0:
				//nearest
				textureNearest(boundingBox, color, depth, v0, v1, v2);
				break;
			case  1:
				//linear
				textureLinear(boundingBox, color, depth, v0, v1, v2);
				break;
			case  2:
				//mipmap
				textureMipMap(boundingBox, color, depth, v0, v1, v2);
				break;
			default:
				//make nearest?
				textureNearest(boundingBox, color, depth, v0, v1, v2);
				break;
			};

		}


		
		template <size_t rows, size_t columns, size_t num_color>
		void RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, float(&color)[rows][columns][num_color], float(&depth)[rows][columns], size_t WINDOW_HEIGHT, size_t WINDOW_WIDTH, bool isTextured, int textureMode) {


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

			BoundingBox boundingBox(_minY, _maxY, _minX, _maxX);
			if (!isTextured) colorMapping(boundingBox, color, depth, v0, v1, v2);
			else textureMapping(boundingBox, color, depth, v0, v1, v2, textureMode);
		}
};
