//
//  Clouds.cpp
//  cse167_final
//
//  Created by Zhaopu Wang on 3/15/20.
//  Copyright © 2020 Zhaopu Wang. All rights reserved.
//

#include "Clouds.h"

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
                float brightness = 1.0f - (float)turbulence(i, j, k, 32) / 256.0f;
                float redPortion = brightness*255.0f;
                float greenPortion = brightness*255.0f;
                float bluePortion = 1.0f*255.0f;
                int base = i*(noiseWidth*noiseHeight*4)+j*(noiseHeight*4)+k*4;
                data[base+0] = (GLubyte)redPortion;
                data[base+1] = (GLubyte)greenPortion;
                data[base+2] = (GLubyte)bluePortion;
                data[base+3] = (GLubyte)255;

            }
        }
    }
}

double Clouds::load3DTexture()
{
    GLuint textureID;
    GLubyte* data = new GLubyte[noiseWidth*noiseHeight*noiseDepth*4];
    
    fillDataArray(data);
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, noiseWidth, noiseHeight, noiseDepth);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, texWidth, texHeight, texDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
    return textureID;
}

double Clouds::smoothNoise(double x1, double y1, double z1)
{
    double fractX = x1 - int(x1);
    double fractY = y1 - int(y1);
    double fractZ = z1 - int(z1);

    int x2 = (int(x1) + noiseWidth + 1) % noiseWidth;
    int y2 = (int(y2) + noiseHeight + 1) % noiseHeight;
    int z2 = (int(z2) + noiseDepth + 1) % noiseDepth;


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

double Clouds::turbulence(double x, double y, double z, double maxZoom)
{
    double sum = 0.0;
    double zoom = maxZoom;
    while (zoom >= 1.0) {
        sum = sum + smoothNoise(x/zomm, y/zoom, z/zoom) * zoom;
        zoom = zoom / 2.0;
    }
    sum = 128.0 * sum / maxZoom;
    return sum
}




