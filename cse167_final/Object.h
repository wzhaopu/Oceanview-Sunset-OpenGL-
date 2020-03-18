#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>

class Object
{
protected:
	glm::mat4 model;
	glm::vec3 color;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    GLuint vao, vbo, vbo2, ebo;
public:
    Object(glm::vec3 co, glm::vec3 pos, glm::vec3 scale);
    ~Object();
    glm::mat4 getModel();
    glm::vec3 getColor();

	void draw();
	void update(int sun);
    void rotate();
    void scale(glm::vec3 scaleVec);
    void translate(glm::vec3 transVec);
    void rotate(float deg, glm::vec3 dir);

};

#endif

