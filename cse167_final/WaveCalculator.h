//
//  WaveCalculator.hpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/10/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#ifndef WaveCalculator_h
#define WaveCalculator_h

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



class WaveCalculator
{
protected:
    int waveNum;
    struct GerstnerWave {
        glm::vec2 direction;
        float amplitude;
        float steepness;
        float frequency;
        float speed;
    } waves[8];  // maximum 8 differentwaves
public:
    WaveCalculator(int num);
    ~WaveCalculator();
    glm::vec3 getPosition(glm::vec2 position, float time);
    glm::vec3 getNormal(glm::vec3 position, float time);
};
#endif /* WaveCalculator_hpp */
