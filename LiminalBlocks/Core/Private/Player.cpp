//
// Created by minec on 3/16/2026.
//

#include "../Public/Player.h"

#include <iostream>

#include "Core/Public/ColliderComponent.h"
#include "Core/Public/InputManager.h"
#include "Core/Public/SubsystemManager.h"
#include "Core/Public/WindowManager.h"
#include "Core/Public/World.h"
#include "SFML/Window/Mouse.hpp"

void Player::Begin() {
    SetAcceleration({0.0f, -8.f, 0.0f});
    SetPosition({0.0f, 60.0f, 0.0f});

    auto inputManager = SubsystemManager::Get()->GetSubsystem<InputManager>();
    inputManager->BindEventToKey({Key::Space, PRESSED}, [this] {
        this->SetVelocity({0, 3, 0});
    });

    auto world = World::GetOrCreate();

    inputManager->BindEventToKey(KeyEvent(Key::Q, KeyEventType::PRESSED), [this, world] {
        auto RayCast = world->RayCastForBlocks(camera->GetPosition(), camera->GetForwardDirection() * 10.f + camera->GetPosition());
        if (RayCast.Block) {
            world->SetBlockAtLocation(RayCast.BlockPosition.x, RayCast.BlockPosition.y, RayCast.BlockPosition.z, Block::CopyBlock<Block>(Blocks::airBlock));
        }
    });

    inputManager->BindEventToKey(KeyEvent(Key::E, KeyEventType::PRESSED), [this, world]() {
        auto RayCast = world->RayCastForBlocks(camera->GetPosition(), camera->GetForwardDirection() * 10.f + camera->GetPosition());
        if (RayCast.Block) {
            auto PlaceLocation = RayCast.BlockPosition + RayCast.Normal;
            world->SetBlockAtLocation(PlaceLocation.x, PlaceLocation.y, PlaceLocation.z, Block::CopyBlock<Block>(Blocks::stoneBlock));
        }
    });

    inputManager->BindEventToKey(KeyEvent(Key::R, KeyEventType::PRESSED), [this, world]() {
        int r = 3;
        int minx = GetPosition().x - r, maxx = GetPosition().x + r;
        int miny = GetPosition().y - r, maxy = GetPosition().y + r;
        int minz = GetPosition().z - r, maxz = GetPosition().z + r;

        for (int x = minx; x <= maxx; x++) {
            for (int y = miny; y <= maxy; y++) {
                for (int z = minz; z <= maxz; z++) {
                    world->SetBlockAtLocation(x, y, z, Blocks::airBlock);
                }
            }
        }
    });


    AddComponent<ColliderComponent>();
}
void Player::Tick(float dt) {
    auto inputManager = SubsystemManager::Get()->GetSubsystem<InputManager>();
    auto& window = SubsystemManager::Get()->GetSubsystem<WindowManager>()->window;

    // 1. Calculate Input Direction
    float LRScalar = inputManager->IsKeyDown(Key::D) * 1.f + inputManager->IsKeyDown(Key::A) * -1.f;
    float FBScalar = inputManager->IsKeyDown(Key::W) * 1.f + inputManager->IsKeyDown(Key::S) * -1.f;

    Vector3f inputDirectionVector =
        LRScalar * camera->GetRightDirection() +
        FBScalar * Vector3f{camera->GetForwardDirection().x, 0, camera->GetForwardDirection().z};

    if (inputDirectionVector.lengthSquared() != 0) {
        inputDirectionVector = inputDirectionVector.normalized();
    }

    // 2. Update Velocity instead of Translating directly
    // This allows the ColliderComponent to handle the movement safely
    Vector3f currentVel = GetVelocity();
    currentVel.x = inputDirectionVector.x * 4.f;
    currentVel.z = inputDirectionVector.z * 4.f;
    SetVelocity(currentVel);

    // 3. Apply Gravity/Acceleration
    ExecutePhysicsTick(dt);

    // 4. Resolve Movement & Collisions
    // This moves the player axis-by-axis and zeros out velocity on hit
    TickComponents(dt);

    // 5. Mouse and Camera Update (must happen AFTER collision resolution)
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float deltaX = (float)mousePos.x - window.getSize().x / 2;
    float deltaY = (float)mousePos.y - window.getSize().y / 2;
    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

    float sensitivity = 0.1f;
    Vector3f currentRot = camera->GetRotation();
    currentRot.y += deltaX * sensitivity;
    currentRot.x -= deltaY * sensitivity;

    if (currentRot.x > 89.0f)  currentRot.x = 89.0f;
    if (currentRot.x < -89.0f) currentRot.x = -89.0f;

    camera->SetRotation(currentRot);
    camera->SetPosition(GetPosition() + Vector3f{0, 0.9f, 0});

    std::cout << GetPosition().x << GetPosition().y << GetPosition().z << std::endl;
}

void Player::AssignCamera(Camera *_camera) {
    this->camera = _camera;
}
