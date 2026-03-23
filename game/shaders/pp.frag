#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

float near = 0.1;
float far  = 200.0;

// Column-major: this is your Sharpen kernel
float a = 0, b = -2, c = 9;

mat3 k = mat3(
a, b, a,
b, c, b,
a, b, a
);

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

vec4 ker(mat3 k)
{
    vec2 size = vec2(textureSize(screenTexture, 0));
    vec4 res = vec4(0.0);
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            vec2 offset = vec2(float(i) / size.x, float(j) / size.y);
            // Use i+1 and j+1 to access mat3 indices 0, 1, 2
            res += texture(screenTexture, TexCoords + offset) * k[i + 1][j + 1];
        }
    }
    return res;
}

void main() {
    vec4 res = ker(k);

    float depthValue = texture(depthTexture, TexCoords).r;
    // Linearize and apply the power curve for a nice atmospheric fade
    float linearDepth = clamp(pow(depthValue, 20), 0, 1);

    // Fog color (Dark Grey)
    vec3 fogColor = vec3(0.4);

    // Mix the sharpened result with the fog based on linearized depth
    vec3 finalCol = mix(res.rgb, fogColor, linearDepth);

    FragColor = vec4(finalCol, 1.0);
}