//
// Created by minec on 3/15/2026.
//

#ifndef OOP_BLOCKS_H
#define OOP_BLOCKS_H
#include "Core/Public/Block.h"

namespace Blocks {
    static Block* airBlock = Block::NewBlock<Block>(0);
    static Block* dirtBlock = Block::NewBlock<Block>(1);
    static Block* grassBlock = Block::NewBlock<Block>(2);
    static Block* stoneBlock = Block::NewBlock<Block>(3);
    static Block* sandBlock = Block::NewBlock<Block>(4);
    static Block* smileOreBlock = Block::NewBlock<Block>(5);
    static Block* frownOreBlock = Block::NewBlock<Block>(6);
}


#endif //OOP_BLOCKS_H
