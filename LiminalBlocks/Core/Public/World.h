//
// Created by minec on 3/13/2026.
//

#ifndef OOP_WORLD_H
#define OOP_WORLD_H
#include <cmath>
#include <vector>

#include "Block.h"
#include "MulticastDelegate.h"
#include "WorldGenerator.h"
#include "../../../data/Blocks.h"
#include "glm/vec3.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"

#define CHUNK_SIZE 32

struct Chunk {
    Block* blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    sf::Vector2i position = {0,0};

    bool validLocation(int x, int y, int z) const {
        return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
    }

    Block* GetBlockAtChunkRelativeLocation(int x, int y, int z) {
        if (validLocation(x, y, z))
            return blocks[x][y][z];
        return nullptr;
    }
    Chunk() {}
};

#define CHUNK_AMOUNT 8

struct VectorHasher {
    size_t operator()(const sf::Vector2i& v) const {
        // A simple bit-shifting hash to combine X and Y
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};

struct RayCastForBlocksData {
    Block* Block;
    sf::Vector3i BlockPosition;
    sf::Vector3i Normal;
    sf::Vector3f HitPosition;
};

class World {
    protected:
    WorldGenerator* worldGenerator = nullptr;

    public:
    MulticastDelegate<Chunk*, Block*> OnChunkChanged;

    std::unordered_map<sf::Vector2i, Chunk*, VectorHasher> chunks;
    World();

    Chunk* GetChunkAtBlockLocation(int x, int y, int z) {
        // Use floor to ensure -1 / 32 becomes -1, not 0
        int cx = std::floor((float)x / CHUNK_SIZE);
        int cz = std::floor((float)z / CHUNK_SIZE);

        if (chunks.contains({cx, cz})) return chunks[{cx, cz}];
        return nullptr;
    }

    Block* GetBlockAtLocation(int x, int y, int z);

    void SetBlockAtLocation(int x, int y, int z, Block* block);

    RayCastForBlocksData RayCastForBlocks(sf::Vector3f startPos, sf::Vector3f endPos);

    static World* GetOrCreate();

protected:
    void InitializeBlockAtLocation(int x, int y, int z, Block* block);
    void InitializeBlockAtLocation(int x, int y, int z, Block* block, Chunk* chunk);
};


#endif //OOP_WORLD_H