#version 330 core

in vec2 tc;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D skyTexture;

void main()
{
    FragColor = vec4(texture(skyTexture, tc).rgb, 1.0f);
    // FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

