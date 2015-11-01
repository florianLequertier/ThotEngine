#version 330 core

const int NB_LIGHT_MAX = 20;

in vec3 vPosition_vs;
in vec2 vTextureCoord;
in vec3 vNormal;
uniform vec3 uPosition_glob;

uniform sampler2D material_texture_diffuse1;
uniform sampler2D material_texture_specular1;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs[NB_LIGHT_MAX];
uniform vec3 uPointLightIntensity[NB_LIGHT_MAX];
uniform vec3 uLightDir_vs[NB_LIGHT_MAX];
uniform vec3 uDirectionnalLightIntensity[NB_LIGHT_MAX];
uniform int uNbPointLight;
uniform int uNbDirectionnalLight;

out vec4 color;



vec3 blinnPhong_dir(vec3 direction, vec3 intensity, vec3 direction_view)
{

    vec3 couleurFinale = vec3(0,0,0);
    vec3 lightDir = direction;
    //Diffuse :
    float diff = max(dot(vNormal, lightDir), 0.0);
    // Specular :
    vec3 reflectDir = reflect(-lightDir, vNormal);
    float spec = pow(max(dot(direction_view, reflectDir), 0.0), uShininess);
    vec3 Li = intensity;

    vec3 ambient = Li * vec3(texture(material_texture_diffuse1, vTextureCoord));
    vec3 diffuse = Li * diff * vec3(texture(material_texture_diffuse1, vTextureCoord));
    vec3 specular = Li * spec * vec3(texture(material_texture_specular1, vTextureCoord));

    couleurFinale = (ambient + diffuse + specular);

    return couleurFinale;
}


vec3 blinnPhong_point(vec3 pos, vec3 intensity, vec3 direction_view)
{
    vec3 couleurFinale = vec3(0,0,0);
    vec3 lightDir = (normalize(pos - vPosition_vs));
    //Diffuse :
    float diff = max(dot(vNormal, lightDir), 0.0);
    // Specular :
    vec3 reflectDir = reflect(-lightDir, vNormal);
    float spec = pow(max(dot(direction_view, reflectDir), 0.0), uShininess);
    float distance = length(pos - vPosition_vs);
    vec3 Li = intensity/(distance * distance);

    vec3 ambient = Li * vec3(texture(material_texture_diffuse1, vTextureCoord));
    vec3 diffuse = Li * diff * vec3(texture(material_texture_diffuse1, vTextureCoord));
    vec3 specular = Li * spec * vec3(texture(material_texture_specular1, vTextureCoord));

    couleurFinale = (ambient + diffuse + specular);

    return couleurFinale;

}


void main()
{
    vec3 direction_view = normalize(uPosition_glob - vPosition_vs);
    vec3 tempColor = vec3(0,0,0);

        for(int i=0; i<uNbDirectionnalLight; i++)
        {
            tempColor += blinnPhong_dir(uLightDir_vs[i], uDirectionnalLightIntensity[i], direction_view );
        }


        for(int i=0; i<uNbPointLight; i++)
        {
            tempColor +=  blinnPhong_point(uLightPos_vs[i], uPointLightIntensity[i], direction_view );
        }

        color = vec4(tempColor, texture(material_texture_diffuse1, vTextureCoord).w);
        //color = baseColor;

};
