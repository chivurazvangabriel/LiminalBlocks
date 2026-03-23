//
// Created by minec on 3/16/2026.
//

#include "../Public/PostProcessingManager.h"
#include "../libs/glad/include/glad/glad.h" // 1. GLAD FIRST
#include <SFML/Window.hpp>               // 2. Then SFML
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Core/Statics/ShaderStatics.h"

float PostProcessingManager::ppQuadVertices[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

PostProcessingManager::PostProcessingManager() {

}

void PostProcessingManager::InitializePostProcessing(const sf::Window& window) {

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.getSize().x, window.getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    // Note the formats: GL_DEPTH_COMPONENT and GL_FLOAT
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window.getSize().x, window.getSize().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Clamp to edge prevents weird wrapping artifacts at the edges of the screen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Attach it to the FBO's depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenVertexArrays(1, &ppVAO);
    glBindVertexArray(ppVAO);

    glGenBuffers(1, &ppVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ppVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(ppQuadVertices), ppQuadVertices, GL_STATIC_DRAW);

    // A float is 4 bytes. Your stride is 4 floats (x, y, u, v)
    GLsizei stride = 4 * sizeof(float);

    glEnableVertexAttribArray(0); // Position (x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1); // TexCoords (u, v)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    screenShader = ShaderStatics::createShaderProgram("pp.vert", "pp.frag");

}

void PostProcessingManager::PreRenderSetup() {

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
}

void PostProcessingManager::PostRenderSetup() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Back to default screen
    glDisable(GL_DEPTH_TEST); // Disable depth so the quad isn't discarded
    glClearColor(1.0f, 0.f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(screenShader);
    glUniform1i(glGetUniformLocation(screenShader, "screenTexture"), 0);
    glUniform1i(glGetUniformLocation(screenShader, "depthTexture"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glBindVertexArray(ppVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
