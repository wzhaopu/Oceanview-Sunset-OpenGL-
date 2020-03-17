#version 330 core

in vec2 tc;
out vec4 fragColor;

uniform float d;

uniform sampler2D skyTexture;

void main()
{
    vec4 texColor = texture(skyTexture, tc);
    fragColor = texColor;

}

