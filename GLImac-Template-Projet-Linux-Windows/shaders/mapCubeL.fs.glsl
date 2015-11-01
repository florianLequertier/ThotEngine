#version 330 core

const int NB_LIGHT_MAX = 20*3;

in float vHasFocus;
in vec3 vPosition_vs;
in vec2 vTextureCoord;
in vec3 vNormal;

uniform sampler2D uTexture;

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


vec3 blinnPhong_point(vec3 pos, vec3 intensity)
{
    vec3 couleurFinale = vec3(0,0,0);
    vec3 wi = (normalize(pos - vPosition_vs));

    if( dot(vNormal, wi) >= 0.0 )
    {
        vec3 w0 = (normalize(-vPosition_vs));
        vec3 halfVector = (wi+w0)/2.f;
        float dist_light_frag = distance(pos,  vPosition_vs);
        vec3 Li = intensity/(dist_light_frag * dist_light_frag);
        couleurFinale = Li*(uKd*( dot(wi, vNormal) ) + uKs*( pow(  dot( halfVector, vNormal), uShininess ) ) );
    }
    return couleurFinale;

}

vec3 blinnPhong_dir(vec3 direction, vec3 intensity)
{
    vec3 couleurFinale = vec3(0,0,0);
    vec3 wi = direction;

    //if(dot(vNormal, wi) >= 0.0)
    {
        vec3 w0 = (normalize(-vPosition_vs));
        vec3 halfVector = (wi+w0)/2.f;
        vec3 Li = intensity;
        couleurFinale = Li*(uKd*( dot(wi, vNormal) ) + uKs*( pow(  dot( halfVector, vNormal), uShininess ) ) );
    }

    return couleurFinale;
}

void main()
{
    vec4 baseColor;
    if(vHasFocus<0.5f)
    {

        baseColor = texture(uTexture, vTextureCoord).xyzw;

        if(baseColor.w == 0 && vHasFocus<-0.2f)
        {
            discard;
        }
        else
        {
            float transparence = baseColor.w;
            color = baseColor/40;


            vec3 tempColor = vec3(0,0,0);
            /*for(int i=0; i<uNbDirectionnalLight; i++)
            {
                tempColor += ( vec3(baseColor) * blinnPhong_dir(uLightDir_vs[i], uDirectionnalLightIntensity[i]) );
            }


            for(int i=0; i<uNbPointLight; i++)
            {
                tempColor +=  ( vec3(baseColor) * blinnPhong_point(uLightPos_vs[i], uPointLightIntensity[i]) );
            }*/


            for(int i=0; i<max(uNbPointLight, uNbDirectionnalLight); i++)
            {
                if(i<uNbDirectionnalLight)
                tempColor += ( vec3(baseColor) * blinnPhong_dir(uLightDir_vs[i], uDirectionnalLightIntensity[i]) )/uNbDirectionnalLight;
                if(i<uNbPointLight && length(uLightPos_vs[i])<20 && distance(uLightPos_vs[i], vPosition_vs)<10)
                tempColor +=  ( vec3(baseColor) * blinnPhong_point(uLightPos_vs[i], uPointLightIntensity[i]) )/uNbPointLight;

            }



            //tempColor/=( uNbDirectionnalLight +uNbPointLight);
            color += vec4(tempColor,0);
            color.w = transparence;

        }


    }
    else
    {
        color = vec4(0,1,0,1);
    }




};



