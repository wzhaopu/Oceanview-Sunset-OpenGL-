#version 330 core

in vec2 tc;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform float d;

uniform sampler3D noise;

void main()
{
    vec4 texColor = texture(noise, vec3(tc.x, tc.y, d));

    FragColor = texColor;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 0.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
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
