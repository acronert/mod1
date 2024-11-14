#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4(diffuse, 1.0);
}
