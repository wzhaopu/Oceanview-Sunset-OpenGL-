#version 330 core

in vec2 tc;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform float d;
uniform float rotAmt;
uniform bool nightShift;
uniform sampler3D noise;

void main()
{
    vec4 texColor = texture(noise, vec3(tc.x, tc.y, d));
    if (!nightShift)
        texColor.rgb -= vec3(0.5f);
    if (rotAmt <= 90.0f)
        texColor.rgb -= vec3(0.006f*rotAmt);
    else if (rotAmt > 90.0f && rotAmt <= 180.0f)
        texColor.rgb -= vec3(0.54f);
    else if (rotAmt > 180.0f && rotAmt < 270.0f) {
        texColor.rgb -= vec3(0.54f);
        texColor.rgb += vec3(0.006f*(rotAmt-180.0f));
    }
    FragColor = texColor;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 0.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
}


