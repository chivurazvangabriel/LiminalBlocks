//
// Created by minec on 3/13/2026.
//

#include "../Public/World.h"

#include "Core/Public/WorldGenerator.h"

World::World() {
    worldGenerator = new WorldGenerator();

    for (int i = 0; i < CHUNK_AMOUNT; i++) {
        for (int j = 0; j < CHUNK_AMOUNT; j++) {
            int ChunkX = i - CHUNK_AMOUNT / 2;
            int ChunkY = j - CHUNK_AMOUNT / 2;

            auto chunk = new Chunk();

            chunk->position = {ChunkX, ChunkY};
            chunks[{ChunkX, ChunkY}] = chunk;

            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    for (int z = 0; z < CHUNK_SIZE; z++) {
                        WorldGeneratorBlockPositionData positionData;
                        positionData.blockPosition = {x, y, z};
                        positionData.chunkPosition = {i, j};
                        auto block = worldGenerator->GetBlockFromData(positionData);


                        InitializeBlockAtLocation(x + ChunkX * CHUNK_SIZE, y, z + ChunkY * CHUNK_SIZE, block, chunk);
                    }
                }
            }
        }
    }
}

Block * World::GetBlockAtLocation(int x, int y, int z) {
    if (auto chunk = GetChunkAtBlockLocation(x, y, z)) {
        int lx = ((x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
        int ly = ((y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
        int lz = ((z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
        return chunk->GetBlockAtChunkRelativeLocation(lx, ly, lz);
    }
    return nullptr;
}

void World::SetBlockAtLocation(int x, int y, int z, Block *block) {
    auto chunk = GetChunkAtBlockLocation(x, y, z);
    InitializeBlockAtLocation(x, y, z, block);
    OnChunkChanged.Broadcast(chunk, GetBlockAtLocation(x, y, z));
}

RayCastForBlocksData World::RayCastForBlocks(sf::Vector3f startPos, sf::Vector3f endPos) {
    RayCastForBlocksData data{nullptr, {0, 0, 0}, {0,0,0}};

    sf::Vector3f dir = (endPos - startPos).normalized();
    float maxDist = (endPos - startPos).length();

    // 1. Initial voxel coordinates (must use floor to handle negative coordinates properly)
    int x = static_cast<int>(std::floor(startPos.x));
    int y = static_cast<int>(std::floor(startPos.y));
    int z = static_cast<int>(std::floor(startPos.z));

    // 2. Determine the direction of the ray on each axis (+1, -1, or 0)
    int stepX = (dir.x > 0) ? 1 : (dir.x < 0 ? -1 : 0);
    int stepY = (dir.y > 0) ? 1 : (dir.y < 0 ? -1 : 0);
    int stepZ = (dir.z > 0) ? 1 : (dir.z < 0 ? -1 : 0);

    // 3. How far we must move along the ray to cross exactly one voxel width on each axis
    // If direction is 0, we set it to max float to prevent division by zero
    float tDeltaX = (stepX != 0) ? std::abs(1.0f / dir.x) : std::numeric_limits<float>::max();
    float tDeltaY = (stepY != 0) ? std::abs(1.0f / dir.y) : std::numeric_limits<float>::max();
    float tDeltaZ = (stepZ != 0) ? std::abs(1.0f / dir.z) : std::numeric_limits<float>::max();

    // 4. How far we must move along the ray to reach the *first* voxel boundary on each axis
    float tMaxX = (stepX > 0) ? (std::floor(startPos.x) + 1.0f - startPos.x) * tDeltaX
                              : (startPos.x - std::floor(startPos.x)) * tDeltaX;
    float tMaxY = (stepY > 0) ? (std::floor(startPos.y) + 1.0f - startPos.y) * tDeltaY
                              : (startPos.y - std::floor(startPos.y)) * tDeltaY;
    float tMaxZ = (stepZ > 0) ? (std::floor(startPos.z) + 1.0f - startPos.z) * tDeltaZ
                              : (startPos.z - std::floor(startPos.z)) * tDeltaZ;

    int nx = stepX > 0 ? -1 : 1;
    int ny = stepY > 0 ? -1 : 1;
    int nz = stepZ > 0 ? -1 : 1;

    // 5. Check the starting block before entering the loop
    Block* block = GetBlockAtLocation(x, y, z);
    if (block && block->isVisible()) {
        data.Block = block;
        data.BlockPosition = {x, y, z};
        data.HitPosition = startPos;
        return data;
    }

    float dist = 0.0f;

    // 6. Traverse the grid
    while (dist <= maxDist) {

        // Find the closest voxel boundary and step into that voxel
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                x += stepX;
                dist = tMaxX;
                tMaxX += tDeltaX;
                data.Normal = {nx, 0, 0};
            } else {
                z += stepZ;
                dist = tMaxZ;
                tMaxZ += tDeltaZ;
                data.Normal = {0, 0, nz};
            }
        } else {
            if (tMaxY < tMaxZ) {
                y += stepY;
                dist = tMaxY;
                tMaxY += tDeltaY;
                data.Normal = {0, ny, 0};
            } else {
                z += stepZ;
                dist = tMaxZ;
                tMaxZ += tDeltaZ;
                data.Normal = {0, 0, nz};
            }
        }

        // If stepping took us past our maximum reach, stop
        if (dist > maxDist) {
            break;
        }

        // Check the newly entered voxel
        block = GetBlockAtLocation(x, y, z);
        if (block && block->isVisible()) {
            data.Block = block;
            data.BlockPosition = {x, y, z};
            data.HitPosition = startPos + (dir * dist);;
            return data;
        }
    }

    // Returns empty data if no block was hit
    return data;
}

World * World::GetOrCreate() {
    static World* world = new World();
    return world;
}


void World::InitializeBlockAtLocation(int x, int y, int z, Block *block) {
    auto chunk = GetChunkAtBlockLocation(x, y, z);
    InitializeBlockAtLocation(x, y, z, block, chunk);
}

void World::InitializeBlockAtLocation(int x, int y, int z, Block *block, Chunk *chunk) {
    int lx = ((x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int ly = ((y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int lz = ((z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    chunk->blocks[lx][ly][lz] = block;

    block->position = sf::Vector3i(lx, ly, lz);
    //block->worldPosition = sf::Vector3i(lx + chunk->position.x * CHUNK_SIZE, ly, lz + chunk->position.y * CHUNK_SIZE);
    block->worldPosition = sf::Vector3i(x,y,z);
}
