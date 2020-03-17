#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform float xRot;
uniform vec3 cameraPos;
uniform sampler2D skybox;

mat3 rotMatX( in float angle ) {
    return mat3(    1.0,             0,              0,
                      0,     cos(angle),    -sin(angle),
                      0,     sin(angle),     cos(angle));
}

mat3 rotMatY( in float angle ) {
    return mat3(    cos(angle),        0,        sin(angle),
                             0,        1.0,             0,
                    -sin(angle),       0,        cos(angle));
}

void main()
{
    // model = glm::rotate(glm::mat4(1), glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
    // model = glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * model;
    // vec3 rotated = rotMatY(radians(150.0f))*Normal;
    // rotated = rotMatX(radians(-30.0f))*rotated;
    // rotated = rotMatX(radians(xRot))*rotated;
    // vec3 rotated = Normal;
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(Position - cameraPos);
    vec3 Reflect = reflect(I, normalize(Normal));
    vec3 rotatedReflect = rotMatY(radians(90.0f))*Reflect;
    rotatedReflect = rotMatX(radians(-120.0f))*Reflect;
    rotatedReflect = rotMatX(radians(xRot*(-1.0f)))*rotatedReflect;
    // vec3 rotatedReflect = Reflect;
    // vec3 Refract = refract(I, normalize(rotated), ratio);
    float pi = 3.14159265359;
    float reflectU = 0.5 + atan(rotatedReflect.x, rotatedReflect.z)/(2*pi);
    float reflectV = 0.5 + 0.5 * rotatedReflect.y;
    vec2 reflectTC = vec2(reflectU,reflectV);

    vec4 reflectColor = vec4(texture(skybox, reflectTC).rgb, 1.0f);
    // vec4 refractColor = vec4(texture(skybox, tc).rgb, 1.0f);
    vec4 oceanBlue = vec4(0.0f, 47.0f/255.0f, 75.0f/255.0f, 1.0f);
    FragColor = mix(reflectColor, oceanBlue, 0.2f);
    // FragColor = oceanBlue;
}
