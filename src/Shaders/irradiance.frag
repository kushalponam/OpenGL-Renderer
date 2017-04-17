#version 330 core


layout (location = 0) out vec4 fColor;

uniform samplerCube cubeMap;
in vec3 texCoords;

const float PI = 3.14159;
void main()
{
	vec3 irradiance = vec3(0.0);
	vec3 normal = normalize(texCoords);

	
	vec3 up = vec3(0.0,1.0,0.0);
	vec3 right = cross(up,normal);
	up = cross(normal,right);

	float sampleDelta = 0.025f;
	float nrSamples = 0.0f;

	for(float phi = 0.0; phi < 2.0*PI ; phi += sampleDelta )
	{
		for( float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			vec3 tangentSample = vec3(sin(theta)* cos(phi), sin(theta)*sin(phi), cos(theta));
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			sampleVec = clamp(sampleVec,-1,1);

			irradiance += texture(cubeMap,sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples ++;
		}
	}

	irradiance = PI * irradiance * (1.0 / float(nrSamples));
	//vec3 temp= texture(cubeMap,texCoords).rgb;

	fColor = vec4(irradiance,1.0f);
}
