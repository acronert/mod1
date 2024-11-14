#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

out vec3 fragNormal;
out vec3 fragPos;

void main() {
    fragPos = position;
    fragNormal = mat3(normalMatrix) * normal;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
