#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec3 aWorldPos; // Absolute block position from VBO

out vec3 Normal;
out vec2 TexCoord;
flat out ivec3 WorldPos; // Must match the fragment shader input

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    Normal = aNormal;
    TexCoord = aTexCoord;
    WorldPos = aWorldPos; // Pass the VBO data to the next stage
}