#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);

    // mapp to [0,1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}