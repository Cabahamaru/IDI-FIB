#version 330 core

in vec3 fcolor;
out vec4 FragColor;
uniform int x_premut;

void main()
{	
    if (x_premut == 1) {
        if (int(gl_FragCoord.y)%10 < 5) FragColor = vec4(1.,1.,1.,1.);
        else FragColor = vec4(0.,0.,0.,1.);
    }
	else FragColor = vec4(fcolor,1);	
}
