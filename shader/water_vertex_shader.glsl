#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in float height;
layout(location = 3) in float depth;
layout(location = 4) in vec3 normal;

out vec3 fragColor;
out float transparency;
out vec4 fragPosLightSpace;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightDir;

void main() {
    // Transform position (using projection, view, model as before)
    vec4 worldPos = model * vec4(position, height, 1.0);
    fragPos = worldPos.xyz;
    gl_Position = projection * view * worldPos;
    
    // Position in light space for shadows
    fragPosLightSpace = lightSpaceMatrix * worldPos;
    
    // Transform normals
    fragNormal = mat3(transpose(inverse(model))) * normal;

    // set lightDir and normalize
    vec3 normalizedLightDir = normalize(lightDir);

    // calculate light intensity
    float intensity = max(dot(normal, -normalizedLightDir), 0.2);

    // calculate transparency
    transparency = min(depth, 0.6);

    fragColor = intensity * color;
}