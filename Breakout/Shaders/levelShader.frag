#version 420
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 color;
uniform sampler2D image;

void main() {
    FragColor = vec4(color, 1.0) * texture(image, TexCoords);
    //FragColor = vec4(texture(image, TexCoords).rgb, 1.0);
    //FragColor = vec4(color, 1.0);
}