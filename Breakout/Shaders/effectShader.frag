#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform bool shake;
uniform bool chaos;
uniform bool confuse;

uniform vec2 offsets[9];
uniform float blur_kernel[9];
uniform int edge_kernel[9];

uniform sampler2D screenTexture;

void main() {
    FragColor = vec4(0.0);

    vec3 samples[9];
    if (chaos || shake) {
        for (int i = 0; i < 9; i++) {
            samples[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
    }

    if (chaos) {
        for (int i = 0; i < 9; i++) {
            FragColor += vec4(samples[i] * edge_kernel[i], 0.0);
        }
        FragColor.a = 1.0;
    }
    else if (confuse) {
        FragColor = vec4(1.0 - texture(screenTexture, TexCoords.st).rgb, 1.0);
    }
    else if (shake) {
        for (int i = 0; i < 9; i++) {
            FragColor += vec4(samples[i] * blur_kernel[i], 0.0);
        }
        FragColor.a = 1.0;
    } else {
        FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
    }
}