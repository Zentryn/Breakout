#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 spriteColor;

void main() {
    FragColor = vec4(texture(image, TexCoords).rgb, 1.0);
    //FragColor = vec4(spriteColor, 1.0);
}