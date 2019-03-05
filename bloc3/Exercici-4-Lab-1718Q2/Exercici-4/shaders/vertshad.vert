#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 colorFocus;

out vec3 matambfragment;
out vec3 matdifffragment;
out vec3 matspecfragment;
out float matshinfragment;

// Valors per als components que necessitem dels focus de llum
//vec3 colFocus = vec3(0.8, 0.8, 0.8);
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
//vec3 posFocus = vec3(1, 1, 1);  // en SCA

out vec4 vertSCO;
out vec3 normalSCO;
out vec3 fcolor;


void main()
{	
    //fcolor = matdiff;
    //gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    vertSCO = (view * TG * vec4 (vertex, 1.0));
    fcolor = colorFocus;
    
    //vec3 normalSCO = inverse (transpose (mat3 (view * TG))) * normal;

    mat3 normalMatrix = inverse (transpose (mat3 (view * TG)));
    normalSCO = normalMatrix * normal;
    
    gl_Position = proj * vertSCO;

    matambfragment = matamb;
    matdifffragment = matdiff;
    matspecfragment = matspec;
    matshinfragment = matshin;
}
