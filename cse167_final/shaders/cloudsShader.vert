#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoords;

out vec2 tc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position =  projection * view * model * vec4(position, 1.0);
    tc = textCoords;
    // uv mapping
    vec3 d = normalize(position);
    float pi = 3.14159265359;
    float u = 0.5 + atan(d.x, d.z)/(2*pi);
    if (u > 0.999f)
        u = 1.0f;
    if (u < 0.021f )
        u = 0.0f;
    float v = 0.5 + 0.5 * d.y;
    // tc = vec2(u,v);
}

/*
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
}
*/
