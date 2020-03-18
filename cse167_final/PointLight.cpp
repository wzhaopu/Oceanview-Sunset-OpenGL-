//
//  PointLight.cpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/17/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#include "PointLight.h"

PointLight::PointLight(glm::vec3 co, glm::vec3 pos)
{
    toWorld = glm::mat4(1.0f);
    color = co;
    position = pos;
}
void PointLight::update(int sun)
{
    if (sun)
        toWorld = glm::rotate(glm::mat4(1), glm::radians(-0.05f), glm::vec3(1.0f, 0.0f, 0.0f)) * toWorld;
    else
        toWorld = glm::rotate(glm::mat4(1), glm::radians(0.05f), glm::vec3(1.0f, 0.0f, 0.0f)) * toWorld;
}
glm::mat4 PointLight::getToWorld()
{
    return toWorld;
}

glm::vec3 PointLight::getColor()
{
    return color;
}

glm::vec3 PointLight::getPosition()
{
    return position;
}
