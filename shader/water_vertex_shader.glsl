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

    vec3 pos = normal;
    gl_Position = projection * view * model * vec4(position, height, 1.0);

    fragColor = color;
}
