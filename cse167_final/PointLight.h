//
//  PointLight.hpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/17/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#ifndef PointLight_h
#define PointLight_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class PointLight
{
private:
    glm::mat4 toWorld;
    glm::vec3 color;
    glm::vec3 position;
public:
    PointLight(glm::vec3 co, glm::vec3 pos);
    ~PointLight();
    void update(int sun);
    glm::mat4 getToWorld();
    glm::vec3 getColor();
    glm::vec3 getPosition();
};
#endif /* PointLight_hpp */
