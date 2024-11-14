#version 330 core

in vec3 fragNormal;
in vec3 fragPos;

uniform vec3 cameraPos;
uniform float refractionRatio;

out vec4 fragColor;

void main() {
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 refractedDir = refract(viewDir, normalize(fragNormal), refractionRatio);

    vec4 waterColor = texture(waterTexture, texCoord);

    fragColor = vec4(waterColor.rgb, 0.8);
}
