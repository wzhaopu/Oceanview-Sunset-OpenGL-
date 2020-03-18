#include "Object.h"

Object::Object(glm::vec3 co, glm::vec3 pos, glm::vec3 sca)
{
    model = glm::mat4(1.0f);
    translate(pos);
    scale(sca);
    color = co;
    FILE * objFile;
    float x, y, z;
    float r, g, b;
    unsigned int t1, t2, t3, t4, t5, t6;
    int c1, c2;

    objFile = fopen("models/sphere.obj", "rb");
    if (objFile == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }
    while (true) {

        c1 = fgetc(objFile);
        if (c1 == EOF) { break; }
        c2 = fgetc(objFile);
        
        if ((c1 == 'v') && (c2 == 'n')) {
            fscanf(objFile, "%f %f %f", &x, &y, &z);
            normals.push_back(glm::vec3(x, y, z));
        
            // cout << "vn " << x << y << z << endl;
        }
        else if ((c1 == 'v') && (c2 == ' ')) {
            fscanf(objFile, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            vertices.push_back(glm::vec3(x, y, z));

            // cout << "v " << x << y << z << endl;
        }
        else if ((c1 == 'f') && (c2 == ' ')) {
            fscanf(objFile, "%u//%u %u//%u %u//%u", &t1, &t2, &t3, &t4, &t5, &t6);
            indices.push_back(t1 - 1);
            indices.push_back(t3 - 1);
            indices.push_back(t5 - 1);
            // cout << "f " << t1 << " " << t3 << " " << t5 << endl;
        }
        else if (c1 == '\n') {
            fgetc(objFile);
        }
    }

    fclose(objFile);
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    // This tells OpenGL which data it should be paying attention to
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        vertices.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // hw2
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);

}

Object::~Object()
{
    // Delete the VBO and the VAO.
    // Failure to delete your VAOs, VBOs and other data given to OpenGL
    // is dangerous and may slow your program and cause memory leaks
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}
glm::mat4 Object::getModel() { return model; }
glm::vec3 Object::getColor() { return color; }

void Object::draw()
{

    // Bind to the VAO.
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Object::update(int sun)
{
    if (sun)
        model = glm::rotate(glm::mat4(1), glm::radians(-0.05f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
    else
        model = glm::rotate(glm::mat4(1), glm::radians(0.05f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
}
void Object::scale(glm::vec3 scaleVec)
{
    model = model * glm::scale(glm::mat4(1.0f), scaleVec);
}

void Object::translate(glm::vec3 transVec)
{
    model = glm::translate(glm::mat4(1.0f), transVec) * model;
}
