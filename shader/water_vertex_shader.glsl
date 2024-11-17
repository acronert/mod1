#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in float height;
layout(location = 3) in vec3 normal;

out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {


    // transform position (mind the order or funny things appear)
    gl_Position = projection * view * model * vec4(position, height, 1.0);

    // set lightDir and normalize
    vec3 lightDir = normalize(vec3(1.0, 1.0, -1.0));

    // transform the normal to world space

    // calculate light intensity
    float intensity = max(dot(normal, -lightDir), 0.0);


    fragColor = intensity * color;


}
