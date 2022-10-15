#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))




Triangle::Triangle()
{
	v[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	c[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);
}

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);

};

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	t[0] = t0;
	t[1] = t1;
	t[2] = t2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

};

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool isTextured)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));
	
	// For textured object
	if (isTextured)
	{
		glEnable(GL_TEXTURE_2D);

		// Avoid modulating the texture by vertex color
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBegin(GL_TRIANGLES);

			glTexCoord2f(t[0].x, t[0].y);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glTexCoord2f(t[1].x, t[1].y);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glTexCoord2f(t[2].x, t[2].y);
			glVertex3f(v[2].x, v[2].y, v[2].z);

		glEnd();

		glDisable(GL_TEXTURE_2D);


	}
	// For object with only vertex color
	else
	{
		glBegin(GL_TRIANGLES);

			glColor3f(c[0].x, c[0].y, c[0].z);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glColor3f(c[1].x, c[1].y, c[1].z);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glColor3f(c[2].x, c[2].y, c[2].z);
			glVertex3f(v[2].x, v[2].y, v[2].z);
		
		glEnd();
	}

}
void Triangle::setColor(std::vector<glm::vec3> traingleColor) {
	this->c[0] = traingleColor[0];
	this->c[1] = traingleColor[1];
	this->c[2] = traingleColor[2];
}

glm::vec3* Triangle::getVertex() {
	return this->v;
}

float Triangle::maxZ() {
	float z0 = v[0].z;
	float z1 = v[1].z;
	float z2 = v[2].z;

	if (z0 >z1 && z0 > z2) return z0;
	else if (z1 > z2) return z1;
	else return z2;
}

float Triangle::minZ() {
	float z0 = v[0].z;
	float z1 = v[1].z;
	float z2 = v[2].z;

	if (z0 < z1 && z0 < z2) return z0;
	else if (z1 < z2) return z1;
	else return z2;
}


float Triangle::min(float z0, float z1, float z2) {
	if (z0 < z1 && z0 < z2) return z0;
	else if (z1 < z2) return z1;
	else return z2;
}

float Triangle::max(float z0, float z1, float z2) {
	if (z0 > z1 && z0 > z2) return z0;
	else if (z1 > z2) return z1;
	else return z2;
}


Baycentric Triangle::baycentricCoordinate(float xPos, float yPos, glm::vec3 A, glm::vec3 B, glm::vec3 C) {

	float alpha = (-1 * ((xPos - B.x) * (C.y - B.y)) + (yPos - B.y) * (C.x - B.x)) /
		(-1 * ((A.x - B.x) * (C.y - B.y)) + (A.y - B.y) * (C.x - B.x));

	float beta = (-1 * ((xPos - C.x) * (A.y - C.y)) + (yPos - C.y) * (A.x - C.x)) /
		(-1 * ((B.x - C.x) * (A.y - C.y)) + (B.y - C.y) * (A.x - C.x));

	float gamma = 1 - alpha - beta;

	return Baycentric(alpha, beta, gamma);


}
