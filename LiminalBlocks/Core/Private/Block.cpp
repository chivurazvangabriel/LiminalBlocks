//
// Created by minec on 3/12/2026.
//

#include "../Public/Block.h"

Block::Block() {
}

Block * Block::setColor(sf::Vector3f _color) {
    this->color = _color;
    return this;
}

Block * Block::setVisibility(bool _visibility) {
    this->visible = _visibility;
    return this;
}

Block * Block::setCollisionEnabled(bool _collisionEnabled) {
    this->collisionEnabled = _collisionEnabled;
    return this;
}

Block * Block::setTextureAtlasID(int _TextureAtlasID) {
    TextureAtlasID = _TextureAtlasID;
    return this;
}
