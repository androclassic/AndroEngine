#version 120

uniform sampler2D image;


varying vec4 color;
varying vec2 texcoord;



void main(void) {

	if(texture2D(image, texcoord.st).r > 0 || 
	   texture2D(image, texcoord.st).g > 0 || 
	   texture2D(image, texcoord.st).b > 0)
	{	
		gl_FragColor =texture2D(image, texcoord.st) ;
		if(color.r > 0 || color.g > 0 || color.b > 0)
				gl_FragColor =gl_FragColor * color;
	}
	else	
			gl_FragColor = color;		
    
}



