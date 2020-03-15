//
//  WaveCalculator.cpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/10/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#include "WaveCalculator.h"

WaveCalculator::WaveCalculator(int num) {
    // default
    waveNum = num;
    waves[0].direction = {0.1f, 0.9f};
    waves[0].amplitude = 0.1f;
    waves[0].steepness = 1.0f;
    waves[0].frequency = 1.0f;
    waves[0].speed = 1.5f;
    waves[1].direction = {0.2f, 0.8f};
    waves[1].amplitude = 0.2f;
    waves[1].steepness = 0.2f;
    waves[1].frequency = 0.4f;
    waves[1].speed = 1.5f;
    waves[2].direction = {0.3f, 0.7f};
    waves[2].amplitude = 0.3f;
    waves[2].steepness = 0.3f;
    waves[2].frequency = 0.3f;
    waves[2].speed = 0.7f;
    waves[3].direction = {0.4f, 0.6f};
    waves[3].amplitude = 0.4f;
    waves[3].steepness = 0.4f;
    waves[3].frequency = 0.4f;
    waves[3].speed = 0.8f;
    waves[4].direction = {-0.5f, -0.5f};
    waves[4].amplitude = 0.5f;
    waves[4].steepness = 0.5f;
    waves[4].frequency = 0.5f;
    waves[4].speed = 0.52f;
    waves[5].direction = {-0.4f, -0.6f};
    waves[5].amplitude = 0.3f;
    waves[5].steepness = 0.3f;
    waves[5].frequency = 0.3f;
    waves[5].speed = 0.2f;
}

glm::vec3 WaveCalculator::getPosition(glm::vec2 position, float time) {
    glm::vec3 new_position = glm::vec3(position.x, 0, position.y);
    for (int i = 0; i < waveNum; i++) {
        float proj = dot(position, waves[i].direction);
        float phase = time * waves[i].speed;
        float theta = proj * waves[i].frequency + phase;
        float height = waves[i].amplitude * sin(theta);
        float maximum_width = waves[i].steepness * waves[i].amplitude;
        float width = maximum_width * cos(theta);
   
        new_position.y += height;
        new_position.x += waves[i].direction.x * width;
        new_position.z += waves[i].direction.y * width;
    }
    return new_position;
}

glm::vec3 WaveCalculator::getNormal(glm::vec3 position, float time) {
    glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
    for (int i = 0; i < waveNum; i++) {
        float proj = dot(glm::vec2(position.x, position.z), waves[i].direction);
        float phase = time * waves[i].speed;
        float psi = proj * waves[i].frequency + phase;
        float Af = waves[i].amplitude * waves[i].frequency;
        float alpha = Af * sin(psi);
        float omega = Af * cos(psi);
    
        normal.y -= waves[i].steepness * alpha;
        normal.x -= waves[i].direction.x * omega;
        normal.z -= waves[i].direction.y * omega;
    }
    return normal;
}


