#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform bool shake;
uniform bool chaos;
uniform bool confuse;
uniform float time;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;

    if (chaos) {
        float strength = 0.3;
        TexCoords = vec2(aTexCoords.x + sin(time) * strength, aTexCoords.y + cos(time) * strength);
    }
    if (confuse) {
        TexCoords = vec2(1.0 - TexCoords.x, 1.0 - TexCoords.y);
    } else {
        TexCoords = aTexCoords;
    }

    if (shake) {
        float strength = 0.005;
        gl_Position.x += cos(time * 50) * strength;
        gl_Position.y += cos(time * 50) * strength;
    }
}