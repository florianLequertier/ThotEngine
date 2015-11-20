#version 330 core

in vec2 v_texCoords;

out vec4 fFragColor;

struct PointLight
{
    vec3 color;
    vec3 position;
    float radius;
    float intensity;
};

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float intensity;
};

const int NB_POINT_LIGHT = 5;
const int NB_DIRECTIONAL_LIGHT = 5;

uniform PointLight u_pointLights[NB_POINT_LIGHT];
uniform DirectionalLight u_directionalLights[NB_DIRECTIONAL_LIGHT];

uniform int u_nbPointLight;
uniform int u_nbDirectionalLight;

uniform vec3 u_viewPosition;



uniform sampler2D u_fragPos;
uniform sampler2D u_normals;
uniform sampler2D u_albedo;
uniform sampler2D u_specShininess;



vec3 computePointLight(int i, vec3 normal, vec3 fragPos, float specular, float shininess)
{
    vec3 lightDir = normalize(u_pointLights[i].position - fragPos);
    float lightDis = length(u_pointLights[i].position - fragPos);
    vec3 viewDir = normalize(u_viewPosition - fragPos);
    vec3 halfDir = normalize(viewDir + lightDir);

    float invRadius = 1.0 / u_pointLights[i].radius;
    float attenuation = u_pointLights[i].intensity/(1.0+invRadius*lightDis+invRadius*lightDis*lightDis);

    float diffuse = max(dot(normal, lightDir),0.0);

    float specularValue = specular * pow(max(dot(viewDir, halfDir),0.0),shininess);

    return (diffuse + specularValue) * u_pointLights[i].color * attenuation;

}

vec3 computeDirectionalLight(int i, vec3 normal, vec3 fragPos, float specular, float shininess)
{
    vec3 lightDir = normalize(-u_directionalLights[i].direction );
    vec3 viewDir = normalize(u_viewPosition - fragPos);
    vec3 halfDir = normalize(viewDir + lightDir);

    float diffuse = max(dot(normal, lightDir),0.0);

    float specularValue = specular * pow(max(dot(viewDir, halfDir),0.0),shininess);

    return (diffuse + specularValue) * u_directionalLights[i].color * u_directionalLights[i].intensity;

}

void main()
{

    vec3 fragPos = texture(u_fragPos, v_texCoords).rgb;
    vec3 albedo = texture(u_albedo, v_texCoords).rgb;
    float specular = texture(u_specShininess, v_texCoords).r;
    float shininess = texture(u_specShininess, v_texCoords).g;
    vec3 normal = normalize(texture(u_normals, v_texCoords).rgb);

    vec3 lighting = vec3(0.01,0.01,0.01); //ambiant

    vec3 pointLighting = vec3(0.0,0.0,0.0);
    for(int i = 0; i < u_nbPointLight; i++)
    {
        pointLighting += computePointLight(i, normal, fragPos, specular, shininess) / u_nbPointLight;
    }

    vec3 directionalLighting = vec3(0.0,0.0,0.0);
    for(int i = 0; i < u_nbDirectionalLight; i++)
    {
        directionalLighting += computeDirectionalLight(i, normal, fragPos, specular, shininess) / u_nbDirectionalLight;
    }

    lighting += pointLighting;
    lighting += directionalLighting;

    fFragColor = vec4(albedo * lighting, 1); //vec4(0, 200, 0, 255);
};
