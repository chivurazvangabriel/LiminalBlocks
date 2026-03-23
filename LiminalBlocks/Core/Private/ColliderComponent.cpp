#include "../Public/ColliderComponent.h"
#include "Core/Public/Object.h"
#include "Core/Public/World.h"
#include <cmath>
#include <iostream>

void ColliderComponent::Begin() {}

void ColliderComponent::Tick(float deltaTime) {
/*
    World* world = World::GetOrCreate();
    Object* owner = GetOwner();
    if (!owner || !world) return;

    sf::Vector3f velocity = owner->GetVelocity();
    sf::Vector3f movement = velocity * deltaTime;

    auto resolveAxis = [&](sf::Vector3f axis, float moveAmount) {
    if (std::abs(moveAmount) < 0.0001f) return;

    owner->Translate(axis * moveAmount);

    // Use a "Skin" (epsilon) to avoid snagging on adjacent axes
    float epsilon = 0.005f;

    for (int i = 0; i < 4; ++i) { // Limit iterations to prevent infinite loops
        Box box = GetBox();

        // Shrink the non-moving axes slightly so we don't "stub our toe"
        // on the floor while moving Z, or on a wall while moving Y.
        Box queryBox = box;
        if (axis.x == 0) queryBox.extent.x -= epsilon;
        if (axis.y == 0) queryBox.extent.y -= epsilon;
        if (axis.z == 0) queryBox.extent.z -= epsilon;

        // Calculate bounds based on the SHRUNK box
        int minX = std::floor(queryBox.center.x - queryBox.extent.x);
        int maxX = std::ceil(queryBox.center.x + queryBox.extent.x);
        int minY = std::floor(queryBox.center.y - queryBox.extent.y);
        int maxY = std::ceil(queryBox.center.y + queryBox.extent.y);
        int minZ = std::floor(queryBox.center.z - queryBox.extent.z);
        int maxZ = std::ceil(queryBox.center.z + queryBox.extent.z);

        bool collisionFound = false;

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                for (int z = minZ; z <= maxZ; ++z) {
                    auto block = world->GetBlockAtLocation(x, y, z);
                    if (block && block->isCollisionEnabled()) {
                        Box blockBox = {{x + 0.5f, y + 0.5f, z + 0.5f}, {0.5f, 0.5f, 0.5f}};

                        // Check overlap using the SHRUNK box
                        if (queryBox.DoesOverlap(blockBox)) {
                            float overlap = box.GetOverlapOnAxis(blockBox, axis);
                            owner->Translate(axis * overlap);

                            sf::Vector3f vel = owner->GetVelocity();
                            if (axis.x != 0) vel.x = 0;
                            if (axis.y != 0) vel.y = 0;
                            if (axis.z != 0) vel.z = 0;
                            owner->SetVelocity(vel);

                            collisionFound = true;
                            break;
                        }
                    }
                }
                if (collisionFound) break;
            }
            if (collisionFound) break;
        }
        if (!collisionFound) break; // Exit early if no more collisions on this axis
    }
};

    resolveAxis({0, 1, 0}, movement.y);
    std::cout << owner->GetVelocity().x << owner->GetVelocity().y << owner->GetVelocity().z << std::endl;
    resolveAxis({1, 0, 0}, movement.x);
    std::cout << owner->GetVelocity().x << owner->GetVelocity().y << owner->GetVelocity().z << std::endl;
    resolveAxis({0, 0, 1}, movement.z);*/


    /*
    World* world = World::GetOrCreate();
    Object* owner = GetOwner();
    if (!owner || !world) return;

    Box box = GetBox();


    sf::Vector3f velocity = owner->GetVelocity();
    sf::Vector3f movement = velocity * deltaTime;
    sf::Vector3f normVelocity;
    if (velocity.lengthSquared() > 0)
        normVelocity = velocity.normalized();
    else return;

    auto RayStart = owner->GetPosition();
    auto dx = movement.x / box.extent.x;
    auto dy = movement.y / box.extent.y;
    auto dz = movement.z / box.extent.z;
    auto m = std::min(std::min(dx, dy), dz);
    Vector3f BoxRayOffset;
    if (dx == m) {
        BoxRayOffset = normVelocity * box.extent.x;
    }
    else if (dy == m) {
        BoxRayOffset = normVelocity * box.extent.y;
    }
    else {
        BoxRayOffset = normVelocity * box.extent.z;
    }
    auto RayEnd = RayStart + movement + BoxRayOffset;
    auto Ray = world->RayCastForBlocks(RayStart,RayEnd);
    if (Ray.Block) {
        auto Pos = Ray.HitPosition;
        auto RevDir = (RayEnd - RayStart).normalized();
        Vector3f Delta = {RevDir.x * box.extent.x, RevDir.y * box.extent.y, RevDir.z * box.extent.z};
        Vector3f Multi = {(float)(abs(Ray.Normal.x) ^ 1), (float)(abs(Ray.Normal.y) ^ 1), (float)(abs(Ray.Normal.z) ^ 1)};

        owner->Translate(movement + BoxRayOffset - (RayEnd - Ray.HitPosition));
        owner->SetVelocity({velocity.x * Multi.x, velocity.y * Multi.y, velocity.z * Multi.z});

    }*/
    /*
    World* world = World::GetOrCreate();
    Object* owner = GetOwner();
    if (!owner || !world) return;

    Box box = GetBox();
    sf::Vector3f velocity = owner->GetVelocity();
    sf::Vector3f movement = velocity * deltaTime;

    sf::Vector3f position = box.center;

    // Helper lambda to test collision at a given position
    auto CollidesAt = [&](const sf::Vector3f& testPos) -> bool
    {
        Box testBox = { testPos, box.extent };

        int minX = std::floor(testBox.center.x - testBox.extent.x);
        int maxX = std::ceil (testBox.center.x + testBox.extent.x);
        int minY = std::floor(testBox.center.y - testBox.extent.y);
        int maxY = std::ceil (testBox.center.y + testBox.extent.y);
        int minZ = std::floor(testBox.center.z - testBox.extent.z);
        int maxZ = std::ceil (testBox.center.z + testBox.extent.z);

        for (int x = minX; x <= maxX; ++x)
            for (int y = minY; y <= maxY; ++y)
                for (int z = minZ; z <= maxZ; ++z)
                {
                    auto block = world->GetBlockAtLocation(x, y, z);
                    if (block && block->isCollisionEnabled())
                    {
                        Box blockBox = {
                            {x + 0.5f, y + 0.5f, z + 0.5f},
                            {0.5f, 0.5f, 0.5f}
                        };

                        if (testBox.DoesOverlap(blockBox))
                            return true;
                    }
                }

        return false;
    };

    position.x += movement.x;
    if (CollidesAt(position))
    {
        position.x -= movement.x;
        velocity.x = 0;
    }
    position.y += movement.y;
    if (CollidesAt(position))
    {
        position.y -= movement.y;
        velocity.y = 0;
    }
    position.z += movement.z;
    if (CollidesAt(position))
    {
        position.z -= movement.z;
        velocity.z = 0;
    }
    owner->SetVelocity(velocity);

    // Move by delta (not absolute position)
    sf::Vector3f deltaMove = position - box.center;
    owner->Translate(deltaMove);*/

    World* world = World::GetOrCreate();
    Object* owner = GetOwner();
    if (!owner || !world) return;

    sf::Vector3f velocity = owner->GetVelocity();
    sf::Vector3f movement = velocity * deltaTime;

    auto resolveAxis = [&](sf::Vector3f axis, float moveAmount) {
    if (std::abs(moveAmount) < 0.0001f) return;

    owner->Translate(axis * moveAmount);

    // Use a "Skin" (epsilon) to avoid snagging on adjacent axes
    float epsilon = 0.005f;

    for (int i = 0; i < 4; ++i) { // Limit iterations to prevent infinite loops
        Box box = GetBox();

        // Shrink the non-moving axes slightly so we don't "stub our toe"
        // on the floor while moving Z, or on a wall while moving Y.
        Box queryBox = box;
        if (axis.x == 0) queryBox.extent.x -= epsilon;
        if (axis.y == 0) queryBox.extent.y -= epsilon;
        if (axis.z == 0) queryBox.extent.z -= epsilon;

        // Calculate bounds based on the SHRUNK box
        int minX = std::floor(queryBox.center.x - queryBox.extent.x);
        int maxX = std::ceil(queryBox.center.x + queryBox.extent.x);
        int minY = std::floor(queryBox.center.y - queryBox.extent.y);
        int maxY = std::ceil(queryBox.center.y + queryBox.extent.y);
        int minZ = std::floor(queryBox.center.z - queryBox.extent.z);
        int maxZ = std::ceil(queryBox.center.z + queryBox.extent.z);

        bool collisionFound = false;

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                for (int z = minZ; z <= maxZ; ++z) {
                    auto block = world->GetBlockAtLocation(x, y, z);
                    if (block && block->isCollisionEnabled()) {
                        Box blockBox = {{x + 0.5f, y + 0.5f, z + 0.5f}, {0.5f, 0.5f, 0.5f}};

                        // Check overlap using the SHRUNK box
                        if (queryBox.DoesOverlap(blockBox)) {
                            float overlap = box.GetOverlapOnAxis(blockBox, axis);
                            owner->Translate(axis * overlap);

                            sf::Vector3f vel = owner->GetVelocity();
                            if (axis.x != 0) vel.x = 0;
                            if (axis.y != 0) vel.y = 0;
                            if (axis.z != 0) vel.z = 0;
                            owner->SetVelocity(vel);

                            collisionFound = true;
                            break;
                        }
                    }
                }
                if (collisionFound) break;
            }
            if (collisionFound) break;
        }
        if (!collisionFound) break; // Exit early if no more collisions on this axis
    }
};

    std::cout << owner->GetVelocity().x << owner->GetVelocity().y << owner->GetVelocity().z << std::endl;
    resolveAxis({1, 0, 0}, movement.x);
    std::cout << owner->GetVelocity().x << owner->GetVelocity().y << owner->GetVelocity().z << std::endl;
    resolveAxis({0, 0, 1}, movement.z);
    resolveAxis({0, 1, 0}, movement.y);
}