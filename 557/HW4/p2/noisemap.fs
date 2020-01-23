#version 410 core                                                 


uniform sampler2D colorMap;//this is the texture
uniform sampler2D noiseMap;//this is the texture

in vec2 pass_TexCoord;//this is the texture coord
//in vec4 pass_Color;
out vec4 color;
//in vec2 texCoord;
//out vec4 Color;
uniform int texture_blend;

void main (void)
{
	//vec4 noiseVec0;
	vec2 noiseVec;

	// Texture function finds the color component for each texture coordinate.
	//noiseVec0=texture(noiseMap, pass_TexCoord);
	//noiseVec = normalize(noiseVec0);

	noiseVec = normalize(texture(noiseMap, pass_TexCoord).xy);

        //noiseVec[0]=noiseVec0[0];
	//noiseVec[0]=noiseVec0[1];
	//noiseVec[1]=noiseVec0[2];

	noiseVec = (noiseVec ) * 0.035;
	//noiseVec = (noiseVec * 2.0 - 1.0) ;
        color = texture(colorMap, pass_TexCoord + noiseVec);


}