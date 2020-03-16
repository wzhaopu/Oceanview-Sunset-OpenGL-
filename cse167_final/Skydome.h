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
    float rotAmt;
    float depth;
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
    // texture generation
    void generateNoise();
    double smoothNoise(double x1, double y1, double z1);
    double turbulence(double x, double y, double z, double maxZoom);
    void fillDataArray(GLubyte data[]);
    int load3DTexture();

public:
    Skydome();
    ~Skydome();
    glm::mat4 getModel();
    void draw();
    void update(GLuint dLoc);
    
};
#endif /* Skydome_hpp */
