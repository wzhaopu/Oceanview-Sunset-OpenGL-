#version 330 core

in vec2 tc;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform float d;
uniform bool nightShift;
uniform sampler3D noise;

void main()
{
    vec4 texColor = texture(noise, vec3(tc.x, tc.y, d));
    if (!nightShift)
        texColor.rgb -= vec3(0.5f);
    FragColor = texColor;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 0.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
}


