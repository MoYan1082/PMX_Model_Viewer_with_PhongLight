#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

layout(std140) uniform Matrices{
	mat4 projMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
};

void main()
{
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);

	FragPos = (modelMatrix * vec4(aPos, 1.0)).xyz;
	Normal = (modelMatrix * vec4(aNormal, 0.0)).xyz;
	TexCoord = aTexCoord;
}