#version 330 core


out vec4 FragColor;
in vec4 vertSCO;
in vec3 normalSCO;

uniform vec4 focusSCO;
uniform vec3 llumAmbient;
//vec3 llumAmbient = vec3(0.2,0.2,0.2);

in vec3 matambfragment;
in vec3 matdifffragment;
in vec3 matspecfragment;
in float matshinfragment;
in vec3 fcolor;

//vec3 fcolor = vec3(0.8,0.8,0.8);

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambfragment;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + fcolor * matdifffragment * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshinfragment == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinfragment);
    return (colRes + matspecfragment * fcolor * shine);
}

void main()
{	   
        vec3 L = normalize(focusSCO.xyz - vertSCO.xyz);
        vec3 fragmentcolor = Phong(normalize(normalSCO),L,vertSCO);
        FragColor = vec4(fragmentcolor,1);
}
