#version 330 core
out float FragColor;

in vec2 TexCoords;


float radius = 0.5;
float occlusion = 0.0;

uniform sampler2D gPosition; // world space position
uniform sampler2D gNormal;
uniform mat4 view;
uniform mat4 projection;
uniform int sampleSize;
uniform vec3 samples[64];

void main()
{
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    fragPos = vec3(view * vec4(fragPos, 1.0f));
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    normal = transpose(inverse(mat3(view))) * normal;
    // create TBN change-of-basis matrix: from tangent-space to normal space
    vec3 helper = vec3(1.0f, 0.0f, 0.0f);
    if (abs(normal[0]) > 0.999) helper = vec3(0.0f, 1.0f, 0.0f);
    vec3 bitangent = normalize(cross(normal, helper));
    vec3 tangent = normalize(cross(normal, bitangent));
    mat3 TBN = mat3(tangent, bitangent, normal);
    for(int i = 0; i < sampleSize; i++) {
        vec3 samplePos = fragPos + radius * TBN * samples[i];
        vec4 offset = projection * vec4(samplePos, 1.0);
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        // get sample depth
        float sampleDepth = (view * vec4(texture(gPosition, offset.xy).xyz, 1.0f)).z;
        occlusion += (sampleDepth >= samplePos.z) ? 1.0 : 0.0;
    }
    occlusion = 1.0 - (occlusion / sampleSize);
    FragColor = occlusion;
}
