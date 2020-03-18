#version 330 core
// ------------ start edit ---------
struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};
struct PointLight {
    vec3 color;
    vec3 position;
};

uniform Material material;
uniform PointLight sunPtLight;
uniform PointLight moonPtLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float att);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float att)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (att * distance);

    // combine results
    vec3 ambient = vec3(0.5) * material.ambient;
    vec3 diffuse = light.color * (diff * material.diffuse);
    vec3 specular = light.color * (spec * material.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
    // return (ambient + diffuse + specular);
}
// -------------- end edit -------------

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec3 Position;

uniform bool nightShift;
uniform float xRot;
uniform vec3 cameraPos;
// uniform sampler2D skybox;
uniform samplerCube skybox;
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
    //vec3 rotated = rotMatY(radians(150.0f))*Normal;
    //rotated = rotMatX(radians(-30.0f))*rotated;
    //rotated = rotMatX(radians(xRot))*rotated;
    // vec3 rotated = Normal;
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(Position - cameraPos);
    vec3 Reflect = reflect(I, normalize(Normal));
    // vec3 rotatedReflect = rotMatY(radians(90.0f))*Reflect;
    // rotatedReflect = rotMatX(radians(-120.0f))*Reflect;
    // rotatedReflect = rotMatX(radians(xRot*(-1.0f)))*rotatedReflect;
    // vec3 rotatedReflect = Reflect;
    // vec3 Refract = refract(I, normalize(rotated), ratio);
    /*
    float pi = 3.14159265359;
    float reflectU = 0.5 + atan(rotatedReflect.x, rotatedReflect.z)/(2*pi);
    float reflectV = 0.5 + 0.5 * rotatedReflect.y;
    vec2 reflectTC = vec2(reflectU,reflectV);
*/
    // vec4 reflectColor = vec4(texture(skybox, reflectTC).rgb, 1.0f);
    vec4 reflectColor = vec4(texture(skybox, Reflect).rgb, 1.0f);
    // vec4 refractColor = vec4(texture(skybox, tc).rgb, 1.0f);
    vec4 oceanBlue = vec4(0.0f, 47.0f/255.0f, 75.0f/255.0f, 1.0f);
    vec4 oceanColor = mix(reflectColor, oceanBlue, 0.2f);
    // --------------- edit -----------------
    vec3 viewDir = normalize(cameraPos - Position);
    vec3 norm = normalize(Normal);
    vec3 sunResult = oceanColor.rgb * CalcPointLight(sunPtLight, norm, Position, viewDir, 0.0015f);
    vec3 moonResult = oceanColor.rgb * CalcPointLight(moonPtLight, norm, Position, viewDir, 0.1f);
    vec3 lightResult = sunResult;
    FragColor = vec4(lightResult, 1.0f);
    if (nightShift)
        FragColor = oceanColor;
    // FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
