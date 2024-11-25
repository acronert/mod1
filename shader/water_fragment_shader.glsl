#version 330 core

in vec3 fragColor;
in float transparency;
in vec4 fragPosLightSpace;
in vec3 fragNormal;
in vec3 fragPos;

uniform sampler2D shadowMap;
uniform vec3 lightDir;

out vec4 FragColor;

float calculateShadow(vec4 fragPosLightSpace) {
    // Perspective division
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Prevent shadow outside light frustum
    if (projCoords.z > 1.0)
        return 1.0;
    
    // Add bias to reduce shadow acne
    float bias = max(0.005 * (1.0 - dot(fragNormal, -lightDir)), 0.0005);
    
    // Simple shadow calculation
    return currentDepth - bias > closestDepth ? 0.5 : 1.0;
}

void main() {
    // Calculate shadow
    float shadow = calculateShadow(fragPosLightSpace);
    
    // Calculate lighting
    vec3 normal = normalize(fragNormal);
    float diff = max(dot(normal, -lightDir), 0.0);
    
    // Combine shadow and lighting with original color
    vec3 lighting = (0.2 + diff * shadow) * fragColor;

    // Maintain original transparency
    FragColor = vec4(lighting, transparency);
}