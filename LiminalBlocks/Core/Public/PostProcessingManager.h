//
// Created by minec on 3/16/2026.
//

#ifndef OOP_POSTPROCESSINGMANAGER_H
#define OOP_POSTPROCESSINGMANAGER_H
#include "SFML/Window/Window.hpp"


class PostProcessingManager {
    unsigned int fbo, textureColorBuffer, depthTexture;
    static float ppQuadVertices[];
    unsigned int ppVAO, ppVBO;
    unsigned int screenShader;
public:
    PostProcessingManager();
    void InitializePostProcessing(const sf::Window& window);
    void PreRenderSetup();
    void PostRenderSetup();
};


#endif //OOP_POSTPROCESSINGMANAGER_H