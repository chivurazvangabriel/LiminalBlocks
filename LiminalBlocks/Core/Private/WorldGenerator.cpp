//
// Created by minec on 3/18/2026.
//

#include "../Public/WorldGenerator.h"
#include "../data/Blocks.h"
#include "../libs/PerlinNoise.hpp"
#include <cmath>

#include "Core/Public/World.h"

Block * WorldGenerator::GetBlockFromData(WorldGeneratorBlockPositionData position_data) {
    Block* blockTemplate;
    int x = position_data.blockPosition.x + position_data.chunkPosition.x * CHUNK_SIZE, y = position_data.blockPosition.y, z = position_data.blockPosition.z + position_data.chunkPosition.y * CHUNK_SIZE;

    static siv::PerlinNoise::seed_type seed = rand() % 100000;
    static siv::PerlinNoise perlinNoise{seed};

    double height01 = perlinNoise.noise2D_01((double)x/10, (double)z/10);

    int seaLevel = 16;
    int sandHeight = seaLevel + 3;
    int stoneLevel = seaLevel - 3;
    int groundHeight = 16 + height01 * 10;

    if (x % 7 == 0 && z % 7 == 0) {
        blockTemplate = Blocks::dirtBlock;
    }
    else {
        if (y > groundHeight) {
            blockTemplate = Blocks::airBlock;
        }
        else if (y == groundHeight) {
            blockTemplate = Blocks::grassBlock;
        }
        else if (y <= stoneLevel) {

            double oreval = perlinNoise.noise3D_01((double)x/4, (double)y/4, (double)z/4);
            if (oreval < 0.2) {
                blockTemplate = Blocks::frownOreBlock;
            }
            else if (oreval > 0.8) {
                blockTemplate = Blocks::smileOreBlock;
            }
            else blockTemplate = Blocks::stoneBlock;
        }
        else if (y <= sandHeight) {
            blockTemplate = Blocks::sandBlock;
        }
        else {
            blockTemplate = Blocks::dirtBlock;
        }
    }

    return Block::CopyBlock<Block>(blockTemplate);
}
