#version 330 core

in vec2 tc;
out vec4 fragColor;

uniform float d;

uniform sampler3D noise;

void main()
{
    vec4 texColor = texture(noise, vec3(tc.x, tc.y, d));
    fragColor = texColor;
    // vec2 dummy = TexCoords;
    // fragColor = vec4(0.0,0.0,1.0,1.0);
}

/*
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;


void main()
{
    vec3 dummy = Position;
    FragColor = vec4(Normal, 1.0f);
}
 */
