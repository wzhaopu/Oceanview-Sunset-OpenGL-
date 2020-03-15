//
//  Planemesh.hpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/10/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#ifndef _OCEANMESH_H_
#define _OCEANMESH_H_

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
#include <vector>
#include <iostream>
#include "WaveCalculator.h"


class OceanMesh
{
protected:
    float width;
    float y_height;
    int divisor;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    GLuint vao, ebo;
    GLuint vbos[2];
    WaveCalculator* wave;
    void generate();

public:
    OceanMesh(float wi, float y, int d, WaveCalculator* wa);
    ~OceanMesh();
    void draw();
    void update(float t);
    void print();
};

#endif /* Planemesh_hpp */
