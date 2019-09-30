#version 330 core
uniform sampler2D u_Sampler1;
uniform sampler2D u_Sampler2;

uniform float u_Time;
uniform vec3 u_LightDirection;
uniform vec3 u_EyePosition;

in vec3 f_World;
in vec3 f_Normal;
in vec2 f_TexCoord;
out vec4 o_Color;

const vec3 DiffuseColor = vec3(0.8, 0.9, 1.0);
const vec3 AmbientColor = vec3(1.0, 0.8, 0.2);
const float SpecExponent = 11.0;
const float SpecIntensity = 0.2;

void main()
{
	vec2 TexCoord = f_TexCoord;

	vec4 Cheryl1 = texture(u_Sampler1, TexCoord);
	vec4 Cheryl2 = texture(u_Sampler2, TexCoord);

	// Calculate light
	// Diffuse Lighting
	float Diffuse = max(-dot(u_LightDirection, f_Normal), 0.0);

	// Specular highlighting
	// Blinn-phong model
	vec3 WorldToEye = normalize(u_EyePosition - f_World);
	vec3 HalfwayVector = normalize(WorldToEye - u_LightDirection);
	float Specular = max(dot(HalfwayVector, f_Normal), 0.0);
	Specular = pow(Specular, SpecExponent) * SpecIntensity;

	// Ambient light
	float Ambient = 0.2;

	o_Color = mix(Cheryl1, Cheryl2, sin(u_Time) * 0.5 + 0.5);
	o_Color.xyz *= (DiffuseColor * Diffuse) + (AmbientColor * Ambient);
	o_Color.xyz += vec3(Specular);

	//o_Color = vec4(f_Normal, 1.0);
	//o_Color = vec4(Diffuse, Diffuse, Diffuse, 1.0);
	//o_Color = vec4(f_World, 1.0);
}