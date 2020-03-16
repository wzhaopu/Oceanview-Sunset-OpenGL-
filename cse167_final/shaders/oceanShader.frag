#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(Position - cameraPos);
    vec3 Reflect = reflect(I, normalize(Normal));
    vec3 Refract = refract(I, normalize(Normal), ratio);
    vec4 reflectColor = vec4(texture(skybox, Reflect).rgb, 1.0f);
    vec4 refractColor = vec4(texture(skybox, Refract).rgb, 1.0f);
    vec4 oceanBlue = vec4(0.0f, 47.0f/255.0f, 75.0f/255.0f, 1.0f);
    FragColor = mix(reflectColor, refractColor, 0.3f);
}
