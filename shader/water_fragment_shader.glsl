#version 330 core

in vec3 fragColor;
in float transparency;

void main() {

    gl_FragColor = vec4(fragColor, transparency);



}