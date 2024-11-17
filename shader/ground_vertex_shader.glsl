#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // transform position (mind the order or funny things appear)
    gl_Position = projection * view * model * vec4(position, 1.0);

    // set lightDir and normalize
    vec3 lightDir = normalize(vec3(0.5, 0.5, -1.0));

    // calculate light intensity
    float intensity = max(dot(normal, -lightDir), 0.2);

    fragColor = intensity * color;
}
