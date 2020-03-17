//
//  Skydome.hpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/15/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#ifndef Skydome_h
#define Skydome_h

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <stdlib.h>
#include <cmath>

class Skydome
{
protected:
    glm::mat4 model;
    float xRot;
    int numVertices;
    int numIndices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    GLuint vao, ebo;
    GLuint vbos[2];
    
    float toRadians(float degrees);
    void generateSphere(int prec);
    void parseFile();
    unsigned char* loadPPM(const char* filename, int& width, int& height);
    int loadTexture();

public:
    Skydome();
    ~Skydome();
    glm::mat4 getModel();
    float getXRot();
    void draw();
    void update();

    
};
#endif /* Skydome_hpp */
