#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 fragColor;
out vec4 fragPosLightSpace;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
    // Transform position
    vec4 worldPos = model * vec4(position, 1.0);
    fragPos = worldPos.xyz;
    gl_Position = projection * view * worldPos;
    
    // Position dans l'espace de lumière pour les shadows
    fragPosLightSpace = lightSpaceMatrix * worldPos;
    
    // Transformer les normales
    fragNormal = mat3(transpose(inverse(model))) * normal;
    
    // Conserver la couleur originale pour le fragment shader
    fragColor = color;
}