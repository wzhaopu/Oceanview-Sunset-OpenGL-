//
//  Planemesh.cpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/10/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#include "OceanMesh.h"

OceanMesh::OceanMesh(float wi, float y, int d, WaveCalculator* wa) {
    wave = wa;
    width = wi;
    y_height = y;
    divisor = d;

    generate();
    std::cout << "vertices size: " << vertices.size() << std::endl;
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    glGenBuffers(1, &ebo);

    // ----------- vertices --------------
    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        vertices.data(), GL_STREAM_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access vertices through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // ------------ normals ---------------
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 normals.data(), GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // ------------ indices --------------
    // Bind to the second VBO. We will use it to store the indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
        &indices[0], GL_STATIC_DRAW);

    // Unbind from the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
}

OceanMesh::~OceanMesh() {
    glDeleteBuffers(2, vbos);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void OceanMesh::generate() {
    
    glm::vec3 m_startingPos = glm::vec3(-width/2.0f,y_height,-width/2.0f);
    unsigned int m_size = divisor;
    float m_sqrWidth = width / divisor;

    glm::vec3 vertex;

    //Calculating Vertices
    for (unsigned int z = 0; z <= m_size; z++)
    {
        for (unsigned int x = 0; x <= m_size; x++)
        {
            vertex.x = (m_startingPos.x + x * m_sqrWidth);
            vertex.y = (m_startingPos.y);
            vertex.z = (m_startingPos.z + z * m_sqrWidth);
            vertices.push_back(vertex);
            normals.push_back(glm::vec3(0.0, 1.0, 0.0));
        }
    }

    //Calculating Indices
    for (unsigned int i = 0; i < m_size; i++) {
        for (unsigned int j = 0; j < m_size; j++) {
            unsigned int offset = i*(m_size+1) + j;
            // bottom left
            indices.push_back(offset);
            indices.push_back(offset+m_size+1);
            indices.push_back(offset+m_size+2);
            // top right
            indices.push_back(offset);
            indices.push_back(offset+m_size+2);
            indices.push_back(offset+1);
        }
    }
}

void OceanMesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OceanMesh::update(float t) {
    std::vector<glm::vec3> new_vertices;
    std::vector<glm::vec3> new_normals;
    glm::vec3 temp_pos;
    glm::vec3 temp_norm;
    for (int i = 0; i < vertices.size(); i++) {
        temp_pos = wave->getPosition(glm::vec2(vertices[i].x,vertices[i].z), t);
        temp_norm = wave->getNormal(temp_pos, t);
        new_vertices.push_back(temp_pos);
        new_normals.push_back(temp_norm);
    }
    // std::cout << vertices[0].x << ", " << vertices[0].y << ", "<< vertices[0].z << " ==> " << first.x << ", " << first.y << ", "<< first.z << "\n";

    // vertices = new_vertices;
    // std::cout << "new vertices size: " << new_vertices.size() << std::endl;
    // rebind
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * new_vertices.size(),
        new_vertices.data(), GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 new_normals.data(), GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OceanMesh::print() {
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << "v " << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << std::endl;
    }
}


