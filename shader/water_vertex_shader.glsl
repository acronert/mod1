#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in float height;
layout(location = 3) in float depth;
layout(location = 4) in vec3 normal;

out vec3 fragColor;
out float transparency;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

    // transform position (mind the order or funny things appear)
    gl_Position = projection * view * model * vec4(position, height, 1.0);

    // set lightDir and normalize
    vec3 lightDir = normalize(vec3(0.4, 0.4, -1.0));

    // calculate light intensity
    float intensity = max(dot(normal, -lightDir), 0.2);


    // calculate transparency
    transparency = min(depth, 0.5);
    //transparency = 0.5;

    fragColor = intensity * color;


}
