//
//  Skydome.cpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/15/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#include "Skydome.h"

GLuint texture;

Skydome::Skydome()
{
    model = glm::mat4(1.0f);
    model = glm::rotate(glm::mat4(1), glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
    model = glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * model;
    // model = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * model;


    xRot = 0.0f;
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
    texture = loadTexture();
    
}

Skydome::~Skydome() {
    glDeleteBuffers(2, vbos);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

float Skydome::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Skydome::generateSphere(int prec) {
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
            vertices[i*(prec + 1) + j] = glm::vec3(x, y, z)*700.0f;
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

void Skydome::parseFile() {
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

void Skydome::draw(){
    glBindVertexArray(vao);
    // glBindTexture(GL_TEXTURE_3D, noiseTexture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Skydome::update(){
    // model = glm::rotate(glm::mat4(1), glm::radians(0.002f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
    // model = glm::rotate(glm::mat4(1), glm::radians(0.01f), glm::vec3(0.0f, -1.0f, 0.0f)) * model;
    // if (xRot < 100.0f || yRot >= 150.0f) {
    model = glm::rotate(glm::mat4(1), glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
        // model = glm::rotate(glm::mat4(1), glm::radians(0.0005f), glm::vec3(0.0f, 0.0f, 1.0f)) * model;
    xRot += 0.1f;
    // if (xRot == 360.0f)
    //    xRot = 0.0f;
    // }
    /*
    else {
        model = glm::rotate(glm::mat4(1), glm::radians(0.02f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
        // model = glm::rotate(glm::mat4(1), glm::radians(0.005f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
        yRot += 0.02f;
    }
     */
    // else xRot = 0.0f;
    // else {
    //     model = glm::rotate(glm::mat4(1), glm::radians(70.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
    //    xRot += 70.0f;
    // }

}

float Skydome::getXRot()
{
    return xRot;
}

glm::mat4 Skydome::getModel()
{
    return model;
}

unsigned char* Skydome::loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;

    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }

    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);

    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);

    // Read maxval:
    do
    {
      retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }

    return rawData;
}

int Skydome::loadTexture()
{
    GLuint textureID;
    unsigned char *data;
    int width, height;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
    // use bilinear interpolation
    data = loadPPM("skybox/texture.ppm", width, height);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Texture failed to load" << std::endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, noiseWidth, noiseHeight, noiseDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
    return textureID;
}
