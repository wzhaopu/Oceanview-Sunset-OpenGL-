//
//  Clouds.hpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/15/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#ifndef Clouds_h
#define Clouds_h

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define noiseWidth 256
#define noiseHeight 256
#define noiseDepth 256
class Clouds
{
protected:
    double noise[noiseHeight][noiseWidth][noiseDepth];
    void generateNoise();
    float smoothNoise(double x1, double y1, double z1);
    double turbulence(double x, double y, double z, double maxZoom);
    void fillDataArray(GLubyte data[]);
    int load3DTexture();
public:
    Clouds();
    ~Clouds();
};

#endif /* Clouds_hpp */
