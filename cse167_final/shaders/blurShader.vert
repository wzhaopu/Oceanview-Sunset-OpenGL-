#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTC;

out vec2 TexCoords;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTC;
}
