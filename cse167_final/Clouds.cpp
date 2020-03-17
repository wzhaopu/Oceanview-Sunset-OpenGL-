//
//  Skydome.cpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/15/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#include "Clouds.h"
const int noiseWidth = 256;
const int noiseHeight = 256;
const int noiseDepth = 256;
double noise[noiseWidth][noiseHeight][noiseDepth];
GLuint noiseTexture;

Clouds::Clouds()
{
    model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, -50.0f, 0.0f)) * model;
    model = model * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.7f, 1.0f));
    model = glm::rotate(glm::mat4(1), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
    depth = 0.01f;
    rotAmt = 0.0f;
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);
    generateSphere(48);
    // parseFile();
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
        vertices.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access vertices through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    

    // ------------ normals ---------------
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * texCoords.size(),
                 texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

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
    
    // textures
    generateNoise();
    noiseTexture = load3DTexture();
    
    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Clouds::~Clouds() {
    glDeleteBuffers(2, vbos);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

float Clouds::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Clouds::generateSphere(int prec) {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); } // std::vector::push_back()
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); } // inserts new element at
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); } // the end of a vector and
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); } // increases the vector size by 1
    // calculate triangle vertices
    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(toRadians(j*360.0f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j*360.0f / prec)) * (float)abs(cos(asin(y)));
            vertices[i*(prec + 1) + j] = glm::vec3(x, y, z)*500.0f;
            texCoords[i*(prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i*(prec + 1) + j] = glm::vec3(x,y,z);
        }
    }
    // calculate triangle indices
    for (int i = 0; i<prec; i++) {
        for (int j = 0; j<prec; j++) {
            indices[6 * (i*prec + j) + 5] = i*(prec + 1) + j;
            indices[6 * (i*prec + j) + 4] = i*(prec + 1) + j + 1;
            indices[6 * (i*prec + j) + 3] = (i + 1)*(prec + 1) + j;
            indices[6 * (i*prec + j) + 2] = i*(prec + 1) + j + 1;
            indices[6 * (i*prec + j) + 1] = (i + 1)*(prec + 1) + j + 1;
            indices[6 * (i*prec + j) + 0] = (i + 1)*(prec + 1) + j;
        }
    }
}

void Clouds::parseFile() {
    FILE * objFile;
    float x, y, z;
    unsigned int v1, u1, n1, v2, u2, n2, v3, u3, n3;
    int c1, c2;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    
    objFile = fopen("models/hemisphere2.obj", "rb");
    if (objFile == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }
    while (true) {

        c1 = fgetc(objFile);
        if (c1 == EOF) { break; }
        if (c1 == '\n') { continue; }
        c2 = fgetc(objFile);
        // cout << char(c1) << char(c2) << endl;

        // if vertex
        if ((c1 == 'v') && (c2 == ' ')) {
            fscanf(objFile, "%f %f %f\n", &x, &y, &z);
            temp_vertices.push_back(glm::vec3(x, y, z));
            // cout << "v " << x << " " << y << " " << z << endl;
        }

        // if normal
        else if ((c1 == 'v') && (c2 == 'n')) {
            fscanf(objFile, "%f %f %f\n", &x, &y, &z);
            temp_normals.push_back(glm::vec3(x, y, z));
            // cout << "vn " << x << " " << y << " " << z << endl;
        }
        else if ((c1 == 'v') && (c2 == 't')) {
            fscanf(objFile, "%f %f\n", &x, &y);
            temp_uvs.push_back(glm::vec2(x, y));
        }
        else if ((c1 == 'f') && (c2 == ' ')) {
            fscanf(objFile, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
                   &v1, &u1, &n1, &v2, &u2, &n2, &v3, &u3, &n3);
            // cout << "f " << v1 << " " << v2 << " " << v3 << endl;

            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);
            uvIndices.push_back(u1-1);
            uvIndices.push_back(u2-1);
            uvIndices.push_back(u3-1);
            normalIndices.push_back(n1 - 1);
            normalIndices.push_back(n2 - 1);
            normalIndices.push_back(n3 - 1);
        }
        /*
        else if (c1 == '\n') {
            fgetc(objFile);
        }
        */
    }
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        /*
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex];
        glm::vec3 normal = temp_normals[normalIndex];
         */
        vertices.push_back(temp_vertices[vertexIndices[i]]*20.0f);
        normals.push_back(temp_normals[normalIndices[i]]);
        texCoords.push_back(temp_uvs[uvIndices[i]]);
        indices.push_back(i);
    }
    fclose(objFile);
}

void Clouds::draw(){
    glBindVertexArray(vao);
    // glBindTexture(GL_TEXTURE_3D, noiseTexture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Clouds::update(GLuint dLoc){
    model = glm::rotate(glm::mat4(1), glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
    model = glm::rotate(glm::mat4(1), glm::radians(0.005f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
    depth += 0.00005f;
    if (depth >= 0.99f) depth = 0.01f; // wrap-around when we get to the end of the texture map
    glUniform1f(dLoc, depth);
}

void Clouds::generateNoise()
{
    for (int x = 0; x < noiseWidth; x++) {
        for (int y = 0; y < noiseHeight; y++) {
            for (int z = 0; z < noiseDepth; z++) {
                noise[x][y][z] = (double)rand() / (RAND_MAX+1.0);
            }
        }
    }
}

void Clouds::fillDataArray(GLubyte data[])
{
    for (int i = 0; i < noiseWidth; i++) {
        for (int j = 0; j < noiseHeight; j++) {
            for (int k = 0; k < noiseDepth; k++) {
                /*
                double turb = turbulence(i, j, k, 32);
                float brightness = 1.0f - (float)turb / 256.0f;
                float redPortion = brightness*255.0f;
                float greenPortion = brightness*255.0f;
                float bluePortion = 1.0f*255.0f;
                int base = i*(noiseWidth*noiseHeight*4)+j*(noiseHeight*4)+k*4;
                data[base+0] = (GLubyte)redPortion;
                data[base+1] = (GLubyte)greenPortion;
                data[base+2] = (GLubyte)bluePortion;
                data[base+3] = (GLubyte)turb;
                */
                
                int base = i*(noiseWidth*noiseHeight*4)+j*(noiseHeight*4)+k*4;
                double turb = turbulence(i, j, k, 32);
                data[base+0] = (GLubyte)255.0f;
                data[base+1] = (GLubyte)255.0f;
                data[base+2] = (GLubyte)255.0f;
                data[base+3] = (GLubyte)(turb-30.0f);
                
               if ((turb-30.0f) < 0.0f)
                  data[base+3] = (GLubyte)0.0f;


            }
        }
    }
}

int Clouds::load3DTexture()
{
    GLuint textureID;
    GLubyte* data = new GLubyte[noiseWidth*noiseHeight*noiseDepth*4];
    
    fillDataArray(data);
    // make sure not bytes are paddes;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // use bilinear interpolation

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, noiseWidth, noiseHeight, noiseDepth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, noiseWidth, noiseHeight, noiseDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
    return textureID;
}

double Clouds::smoothNoise(double x1, double y1, double z1)
{
    double fractX = x1 - int(x1);
    double fractY = y1 - int(y1);
    double fractZ = z1 - int(z1);

    int x2 = (int(x1) + noiseWidth + 1) % noiseWidth;
    int y2 = (int(y1) + noiseHeight + 1) % noiseHeight;
    int z2 = (int(z1) + noiseDepth + 1) % noiseDepth;


   //smooth the noise with bilinear interpolation
    double value = 0.0;
    value += (1-fractX) * (1-fractY) * (1-fractZ) * noise[(int)x1][(int)y1][(int)z1];
    value += (1-fractX) * fractY     * (1-fractZ) * noise[(int)x1][(int)y2][(int)z1];
    value += fractX     * (1-fractY) * (1-fractZ) * noise[(int)x2][(int)y1][(int)z1];
    value += fractX     * fractY     * (1-fractZ) * noise[(int)x2][(int)y2][(int)z1];
    
    value += (1-fractX) * (1-fractY) * fractZ     * noise[(int)x1][(int)y1][(int)z2];
    value += (1-fractX) * fractY     * fractZ     * noise[(int)x1][(int)y2][(int)z2];
    value += fractX     * (1-fractY) * fractZ     * noise[(int)x2][(int)y1][(int)z2];
    value += fractX     * fractY     * fractZ     * noise[(int)x2][(int)y2][(int)z2];

    return value;
}

double Clouds::turbulence(double x, double y, double z, double size)
{
    double value = 0.0;
    double initialSize = size;
    double cloudQuant;
    while (size >= 0.9) {
        value = value + smoothNoise(x/size, y/size, z/size)*size;
        size = size / 2.0;
    }
    cloudQuant = 140.0;
    value = value /initialSize;
    double temp = value * 128.0 - cloudQuant;
    temp = (1.0/(1.0+pow(2.718, -0.2*temp)));
    value = 256.0 * temp;
    return value;
}

glm::mat4 Clouds::getModel()
{
    return model;
}
