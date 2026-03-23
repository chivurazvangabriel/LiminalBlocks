#include "libs/glad/include/glad/glad.h" // 1. GLAD FIRST
#include <SFML/Window.hpp>               // 2. Then SFML
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Your custom headers
#include "LiminalBlocks/Core/Public/InputManager.h"
#include "Core/Public/Camera.h"
#include "Core/Public/GameInstance.h"
#include "Core/Public/TickManager.h"

// Math
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include "Core/Statics/ColorStatics.h"

#include "Core/Public/Block.h"
#include "Core/Public/BlockRenderer.h"
#include "Core/Public/BlockRenderer.h"
#include "Core/Public/BlockRenderer.h"
#include "Core/Public/Game.h"
#include "Core/Public/Player.h"
#include "Core/Public/PostProcessingManager.h"
#include "Core/Public/SubsystemManager.h"
#include "Core/Public/WindowManager.h"
#include "Core/Public/Game.h"
#include "Core/Public/World.h"

class Block;
using std::cout;

glm::vec3 to_vec3(Vector3f vector) {
    return glm::vec3(vector.x, vector.y, vector.z);
}


void initOpenGL(sf::Vector2u windowSize) {
    // Tells OpenGL the size of the rendering window
    glViewport(0, 0, windowSize.x, windowSize.y);

    // Modern OpenGL needs this to handle 3D depth correctly
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Optional: Cull back faces (don't render the inside of the cube) for better performance
    glEnable(GL_CULL_FACE);
}

#define dt TickManager::DeltaTime

/*RayCastForBlocksData RayCastForBlocks(World* world, Vector3f startPos, Vector3f endPos) {
    RayCastForBlocksData data;
    data.Block = nullptr;
    data.BlockPosition = {};

    Vector3f dir = (endPos - startPos).normalized();
    auto dist = (startPos - endPos).length();
    int
    dx = dir.x > 0 ? 1 : -1,
    dy = dir.y > 0 ? 1 : -1,
    dz = dir.z > 0 ? 1 : -1;

    Vector3f curPos = startPos;
    Block* block = world->GetBlockAtLocation(curPos.x, curPos.y, curPos.z);
    while (!block || !block->isVisible()) {
        if (dist < (curPos - startPos).length()) {
            data.Block = nullptr;
            return data;
        }

        float xAdv = abs(dir.x / (curPos.x - ceil(curPos.x)));
        float yAdv = abs(dir.y / (curPos.y - ceil(curPos.y)));
        float zAdv = abs(dir.z / (curPos.z - ceil(curPos.z)));
        float m = std::max(std::max(xAdv, yAdv), zAdv);
        if (m == xAdv) {
            curPos += dir * xAdv;
            block = world->GetBlockAtLocation(curPos.x + dx, curPos.y, curPos.z);
        }
        else if (m == yAdv) {
            curPos += dir * yAdv;
            block = world->GetBlockAtLocation(curPos.x, curPos.y + dy, curPos.z);
        }
        else {
            curPos += dir * zAdv;
            block = world->GetBlockAtLocation(curPos.x, curPos.y, curPos.z + dz);
        }
    }

    data.Block = block;
    data.BlockPosition = {(int)curPos.x, (int)curPos.y, (int)curPos.z};
    return data;
}*/

#include <limits>
#include <cmath>

int main()
{
    srand(time(0));

    auto* world = World::GetOrCreate();

    auto* subsystemManager = SubsystemManager::Get();

    auto windowManager = subsystemManager->RegisterNewSubsystem<WindowManager>();
    auto& window = windowManager->InitializeWindow();

    auto gameInstance = subsystemManager->RegisterNewSubsystem<GameInstance>();
    auto inputManager = subsystemManager->RegisterNewSubsystem<InputManager>();
    auto tickManager = subsystemManager->RegisterNewSubsystem<TickManager>();
    auto blockRenderer = subsystemManager->RegisterNewSubsystem<BlockRenderer>();
    blockRenderer->Init(world);

    auto* postProcessingManager = new PostProcessingManager();
    postProcessingManager->InitializePostProcessing(window);

    auto* camera = gameInstance->CreateObject<Camera>();
    auto* player = gameInstance->CreateObject<Player>();
    player->AssignCamera(camera);

    initOpenGL(window.getSize());

    sf::Clock clock;

    Block* highlightedBlock = nullptr;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        dt = clock.restart().asSeconds();
        TickManager::Time += dt;

        inputManager->Tick(dt);
        for (auto& Obj : gameInstance->objects) {
            if (auto Tickable = dynamic_cast<ITickable*>(Obj)) {
                Tickable->Tick(dt);
            }
        }

        // Game start


        // Camera Input

        // Render
        postProcessingManager->PreRenderSetup();

        // 1. Clear the screen with a visible color (Dark Blue/Grey)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. Activate Shader
        glUseProgram(BlockRenderer::shaderProgram);

        glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)window.getSize().x / (float)window.getSize().y , 0.1f, 200.0f);
        glUniformMatrix4fv(glGetUniformLocation(BlockRenderer::shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glm::vec3 cameraPos(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
        auto lookTarget = to_vec3(camera->GetForwardDirection() + camera->GetPosition());
        glm::mat4 view = glm::lookAt(cameraPos, lookTarget, glm::vec3(0, 1, 0));
        glUniformMatrix4fv(glGetUniformLocation(BlockRenderer::shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));


        auto RayCast = world->RayCastForBlocks(camera->GetPosition(), camera->GetForwardDirection() * 10.f + camera->GetPosition());
        highlightedBlock = RayCast.Block;
        blockRenderer->HighlightBlock(highlightedBlock);
        for (auto const& [pos, chunk] : world->chunks) {
            blockRenderer->DrawChunk(chunk, world);
        }

        postProcessingManager->PostRenderSetup();

        // 6. Display the finished frame
        window.display();
    }

    delete gameInstance;
    delete inputManager;
    delete tickManager;
    delete blockRenderer;
    delete world;
    return 0;
}
