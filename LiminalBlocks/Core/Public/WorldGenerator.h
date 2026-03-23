//
// Created by minec on 3/18/2026.
//

#ifndef OOP_WORLDGENERATOR_H
#define OOP_WORLDGENERATOR_H
#include "Block.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

using sf::Vector3i, sf::Vector2i;

struct WorldGeneratorBlockPositionData {
    Vector3i blockPosition;
    Vector2i chunkPosition;

};

class WorldGenerator {
    virtual ~WorldGenerator() = default;
    public:
    virtual Block* GetBlockFromData(WorldGeneratorBlockPositionData position_data);
};


#endif //OOP_WORLDGENERATOR_H