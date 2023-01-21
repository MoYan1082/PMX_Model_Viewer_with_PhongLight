#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition; // world space
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform samplerCube shadowMap;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float near_plane;
uniform float far_plane;
uniform mat4 projection;
uniform mat4 view;

float ShadowCalculation(vec3 fragPos, float bias)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    // PCF
    float shadow = 0.0;
    float samples = 4.0;
    float offset = 0.1;
    for (float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(shadowMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= far_plane;   // undo mapping [0;1]
                if (currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
    return shadow;
}

void main()
{
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).xyz;
    vec3 Normal = texture(gNormal, TexCoords).xyz;
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    // set depth
    vec4 tmpPos = projection * view * vec4(FragPos, 1.0);
    tmpPos.xyz /= tmpPos.w;
    //tmpPos.z = tmpPos.z / 2 + 0.5;
    gl_FragDepth = tmpPos.z;
    FragColor = vec4(1.0) * tmpPos.z;
    //// ssao
    //float AmbientOcclusion = texture(ssao, TexCoords).r;
    //// ambient
    //vec3 ambient = 0.2 * Albedo * AmbientOcclusion;
    //// diffuse
    //vec3 lightDir = normalize(lightPos - FragPos);
    //vec3 normal = normalize(Normal);
    //float diff = max(dot(lightDir, normal), 0.0);
    //vec3 diffuse = diff * Albedo * lightColor * AmbientOcclusion;
    //// specular
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    //vec3 specular = vec3(0.3) * spec * lightColor;
    //// shadow map
    //float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
    //float shadow = ShadowCalculation(FragPos, bias);
    //// sum
    //FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), 1.0);
}