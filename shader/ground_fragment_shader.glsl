#version 330 core

in vec3 fragColor;

// out vec4 finalColor;

void main() {
    // finalColor = vec4(fragColor, 0.7);

    gl_FragColor = vec4(fragColor, 1.0);
}