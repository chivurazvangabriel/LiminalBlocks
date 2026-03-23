//
// Created by minec on 3/12/2026.
//

#ifndef OOP_BLOCK_H
#define OOP_BLOCK_H
#include <fstream>
#include <string>

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector3.hpp"
#include "nlohmann/json.hpp"


class Block {
    bool visible = true;
    bool collisionEnabled = true;

    public:
    sf::Vector3f color;
    sf::Vector3i position;
    sf::Vector3i worldPosition;
    int TextureAtlasID;

    Block();
    virtual ~Block() = default;

    [[nodiscard]] virtual bool isVisible() const {return visible;}
    [[nodiscard]] virtual bool isCollisionEnabled() const {return collisionEnabled;}

    // Builder
    Block* setColor(sf::Vector3f _color);
    Block* setVisibility(bool _visibility);
    Block* setCollisionEnabled(bool _collisionEnabled);
    Block* setTextureAtlasID(int _TextureAtlasID);

    template<typename T>
    static T* CopyBlock(Block* templateBlock);

    template<typename T>
    static T* NewBlock(int blockId);
};

template<typename T>
T* Block::CopyBlock(Block* templateBlock) {
    return new T(*static_cast<T*>(templateBlock));
}

using json = nlohmann::json;

template<typename T>
T* Block::NewBlock(int blockId) {
    static json blocks = [] {
        std::ifstream file("resources/Blocks.json");
        if (!file)
            throw std::runtime_error("Cannot open Blocks.json");
        return json::parse(file);
    }();

    for (const auto& block : blocks) {
        if (block.contains("block_id") && block["block_id"] == blockId) {

            auto newBlock = new T();

            newBlock
                ->setTextureAtlasID(block.value("texture_atlas_id", 0))
                ->setCollisionEnabled(block.value("collision", true))
                ->setVisibility(block.value("visible", true));

            return newBlock;
        }
    }

    return nullptr;
}


#endif //OOP_BLOCK_H
