in vec3 v_color;
in vec2 v_texCoords;
in vec3 v_normals;

in vec3 v_fragPos;

out vec4 fFragColor;

struct PointLight
{
    vec3 color;
    vec3 position;
    float radius;
};

struct DirectionalLight
{
    vec4 color;
    vec3 direction;
};

int NB_POINT_LIGHT = 20;
int NB_DIRECTIONAL_LIGHT = 20;

uniform PointLight u_pointLights[NB_POINT_LIGHT];
uniform DirectionalLight u_directionalLights[NB_DIRECTIONAL_LIGHT];

uniform int u_nbPointLight;
uniform int u_nbDirectionalLight;

uniform vec3 u_viewPosition;

uniform float u_specularValue;
uniform float u_shininessValue;

uniform sampler2D u_diffuse;
//TODO : lightmaps... and deferred shading

vec3 computePointLight(int i)
{
    vec3 norm = normalize(v_normals);
    vec3 lightDir = normalize(u_pointLights[i].position - v_fragPos);
    float lightDis = length(u_pointLights[i].position - v_fragPos);
    vec3 viewDir = normalize(u_viewPosition - v_fragPos);

    float invRadius = 1.0 / u_pointLights[i].radius;
    float attenuation = 1.0/(1.0+invRadius*lightDis+invRadius*lightDis*lightDis);

    vec3 reflectDir = reflect(-lightDir, norm);

    float diffuse = max(dot(norm, lightDir),0.0);

    float specular = u_specularValue*pow(max(dot(viewDir, reflectDir),0.0),32);

    return (diffuse + specular) * u_pointLights[i].color * attenuation;

}

void main()
{


    vec4 texColor = texture(u_diffuse, v_texCoords);
    fFragColor = vec4(v_color * vec3(texColor), texColor.a); //vec4(0, 200, 0, 255);
};
