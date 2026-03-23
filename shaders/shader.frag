#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
flat in ivec3 WorldPos; // Data from the Vertex Shader

uniform sampler2D ourTexture;
uniform ivec3 uHighlightPos; // Position of block player is looking at

void main() {
    // 1. Lighting
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);

    // 2. Texture & Color
    vec4 texColor = texture(ourTexture, TexCoord);
    vec3 ambient = 0.3 * texColor.rgb;
    vec3 diffuse = diff * texColor.rgb;
    vec4 PreHighlightColor = vec4(ambient + diffuse, texColor.a);

    // 3. Highlight Logic
    // Compare this specific pixel's block ID to the hovered block ID
    if(uHighlightPos == WorldPos) {
        FragColor = PreHighlightColor * 1.4; // Make it noticeably brighter
    } else {
        FragColor = PreHighlightColor;
    }
}