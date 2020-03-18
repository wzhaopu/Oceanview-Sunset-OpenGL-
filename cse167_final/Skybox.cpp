#include "Skybox.h"

float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Skybox::Skybox()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    model = glm::mat4(1.0f);
    /*
    std::vector<const char*> faces
    {
        "skybox/right.ppm",
        "skybox/left.ppm",
        "skybox/top.ppm",
        "skybox/top.ppm",
        "skybox/front.ppm",
        "skybox/back.ppm"
    };
     
    std::vector<const char*> faces
    {
        "skybox/sea/calm_sea_rt.ppm",
        "skybox/sea/calm_sea_lf.ppm",
        "skybox/sea/calm_sea_up.ppm",
        "skybox/sea/calm_sea_dn.ppm",
        "skybox/sea/calm_sea_ft.ppm",
        "skybox/sea/calm_sea_bk.ppm"
    };
     */
     std::vector<const char*> dayFaces
        {
            "skybox/day/front_1.ppm",
            "skybox/day/front_1.ppm",
            "skybox/day/up_1.ppm",
            "skybox/day/down.ppm",
            "skybox/day/front_1.ppm",
            "skybox/day/front_1.ppm"
        };
        dayTexture = loadCubemap(dayFaces);
    
    // else {
        std::vector<const char*> nightFaces
        {
            "skybox/night/right.ppm",
            "skybox/night/left.ppm",
            "skybox/night/up.ppm",
            "skybox/night/down.ppm",
            "skybox/night/front.ppm",
            "skybox/night/back.ppm"
        };
        nightTexture = loadCubemap(nightFaces);
    // } 
     
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    // This tells OpenGL which data it should be paying attention to
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    for (int i = 0; i < 108; i++) {
        skyboxVertices[i] = skyboxVertices[i] * 500.0f;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices, GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

Skybox::~Skybox()
{
    // Delete the VBO and the VAO.
    // Failure to delete your VAOs, VBOs and other data given to OpenGL
    // is dangerous and may slow your program and cause memory leaks
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Skybox::draw(int nightshift)
{
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
    if (nightshift == 1)
        glBindTexture(GL_TEXTURE_CUBE_MAP, dayTexture);
    else
        glBindTexture(GL_TEXTURE_CUBE_MAP, nightTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

unsigned int Skybox::loadCubemap(std::vector<const char*> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height;
    unsigned char *data;
    
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        data = loadPPM(faces[i], width, height);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }
    }
    // make sure not bytes are paddes;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // use bilinear interpolation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // use clamp to edge to hide skybox edges
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
unsigned char* Skybox::loadPPM(const char* filename, int& width, int& height)
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

