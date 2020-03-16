#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//For loading images
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <stdlib.h>

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//new
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

class Skybox
{
private:
    glm::mat4 model;
    GLuint vao, vbo;
    GLuint projectionLoc, viewLoc;
    unsigned int dayTexture;
    unsigned int nightTexture;
public:
    Skybox();
    ~Skybox();
    
    void draw(int nightshift);
    unsigned char* loadPPM(const char* filename, int& width, int& height);
    unsigned int loadCubemap(std::vector<const char*> faces);
};


#endif /* Skybox_h */
