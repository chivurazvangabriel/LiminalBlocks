//
// Created by minec on 3/12/2026.
//

#include "../Public/BlockRenderer.h"

#include <fstream>
#include <iostream>
#include <ranges>

unsigned int BlockRenderer::shaderProgram = 0;
unsigned int BlockRenderer::TextureAtlasID = 0;

void BlockRenderer::CreateChunkRenderingData(Chunk *chunk, World* world) {
    ChunkRenderingData data;

    glGenVertexArrays(1, &data.VAO);
    glBindVertexArray(data.VAO);

    glGenBuffers(1, &data.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);

    for (int i = 0; i < CHUNK_SIZE; i++) { // x
        for (int j = 0; j < CHUNK_SIZE; j++) { // y
            for (int k = 0; k < CHUNK_SIZE; k++) { // z

                auto GetBlockWorldPosition = [chunk](int i, int j, int k) {
                    return glm::ivec3(i + chunk->position.x * CHUNK_SIZE, j, k + chunk->position.y * CHUNK_SIZE);
                };

                auto BlockWorldPosition = GetBlockWorldPosition(i, j, k);

                auto GetBlockAt = [world, chunk, GetBlockWorldPosition](int i, int j, int k) {
                    auto wPos = GetBlockWorldPosition(i, j, k);
                    return world->GetBlockAtLocation(wPos.x, wPos.y, wPos.z);
                };
                auto Block = GetBlockAt(i, j, k);

                auto validAndVisible = [world, chunk, GetBlockAt](int i, int j, int k) {
                    if (auto Block = GetBlockAt(i, j, k))
                        if (Block->isVisible())
                            return true;

                    return false;
                };

                if (validAndVisible(i, j, k)) {
                    auto blockVisibility = static_cast<RelativeBlockVisibility>(
                        !validAndVisible(i, j - 1, k) << 0 |
                        !validAndVisible(i, j + 1, k) << 1 |
                        !validAndVisible(i - 1, j, k) << 2 |
                        !validAndVisible(i + 1, j, k) << 3 |
                        !validAndVisible(i, j, k - 1) << 4 |
                        !validAndVisible(i, j, k + 1) << 5
                        );

                    auto addQuad = [&blockVisibility, &data, i, j, k, &Block, &BlockWorldPosition](RelativeBlockVisibility checkedVisibility, Quad quad)  {
                        if (blockVisibility & checkedVisibility) {
                            Quad q = Quad::Translate(quad, {i, j, k});
                            q = Quad::ScaleUVs(q, 8, Block->TextureAtlasID);
                            q.SetWorldPosition(BlockWorldPosition);
                            data.quads.push_back(q);
                        }

                    };

                    addQuad(BOTTOM, BlockFaceData::quad_bottom);
                    addQuad(TOP, BlockFaceData::quad_top);
                    addQuad(LEFT, BlockFaceData::quad_left);
                    addQuad(RIGHT, BlockFaceData::quad_right);
                    addQuad(FRONT, BlockFaceData::quad_front);
                    addQuad(BACK, BlockFaceData::quad_back);
                }
            }
        }
    }

    glBufferData(GL_ARRAY_BUFFER, data.quads.size() * sizeof(Quad), data.quads.data(), GL_STATIC_DRAW);

    GLsizei stride = sizeof(Vertex);

    // Position: Offset 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));

    // Normal: Offset is the size of the position vector (3 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texCoord));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 3, GL_INT, stride, (void*)offsetof(Vertex, worldPosition));

    data.dirty = false;
    chunkRenderingData[chunk] = data;
}

void BlockRenderer::Init(World* world) {
    shaderProgram = createShaderProgram("shader.vert", "shader.frag");
    LoadTextures();

    this->world = world;
    world->OnChunkChanged.Bind(this, &BlockRenderer::MakeChunkDirtyAfterBlockChange);
}

void BlockRenderer::MakeChunkDirty(Chunk *chunk) {
    if (!chunk)
        return;

    if (!chunkRenderingData.contains(chunk))
        return;

    chunkRenderingData[chunk].dirty = true;
}

void BlockRenderer::MakeChunkDirtyAfterBlockChange(Chunk *chunk, Block *block) {
    MakeChunkDirty(chunk);

    auto pos = block->worldPosition;

    if (pos.x == 0)
        MakeChunkDirty(world->GetChunkAtBlockLocation(block->worldPosition.x - 1, block->worldPosition.y, block->worldPosition.z));
    else if (pos.x == CHUNK_SIZE - 1)
        MakeChunkDirty(world->GetChunkAtBlockLocation(block->worldPosition.x + 1, block->worldPosition.y, block->worldPosition.z));

    if (pos.y == 0)
        MakeChunkDirty(world->GetChunkAtBlockLocation(block->worldPosition.x, block->worldPosition.y, block->worldPosition.z - 1));
    else if (pos.y == CHUNK_SIZE - 1)
        MakeChunkDirty(world->GetChunkAtBlockLocation(block->worldPosition.x, block->worldPosition.y, block->worldPosition.z + 1));
}

void BlockRenderer::LoadTextures() {
    sf::Image image;
    if (!image.loadFromFile("resources/Atlas.png")) return;

    // SFML images are upside down compared to OpenGL's coordinate system
    // image.flipVertically();

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set wrapping/filtering options (Important for Pixels/Voxels!)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // GL_NEAREST gives that crisp "Minecraft" pixel look
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload the raw pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    TextureAtlasID = textureID;
}

void BlockRenderer::HighlightBlock(Block *block) {
    highlightedBlock = block;
}

void BlockRenderer::Tick(float deltaTime) {

}

void BlockRenderer::DrawChunk(Chunk *chunk, World* world) {
    if (!chunkRenderingData.contains(chunk) || chunkRenderingData[chunk].dirty)
        CreateChunkRenderingData(chunk, world);

    glm::mat4 model = glm::mat4(1.0f);
    int x = chunk->position.x * CHUNK_SIZE, y = 0, z = chunk->position.y * CHUNK_SIZE;
    model = glm::translate(model, glm::vec3(x, y, z));

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    if (highlightedBlock) {
        unsigned int highlightLocationLoc = glGetUniformLocation(shaderProgram, "uHighlightPos");
        auto vptr = glm::value_ptr(glm::ivec3{highlightedBlock->worldPosition.x, highlightedBlock->worldPosition.y, highlightedBlock->worldPosition.z});
        glUniform3iv(highlightLocationLoc, 1, vptr);
    }

    auto& data = chunkRenderingData[chunk];

    glBindVertexArray(data.VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureAtlasID);

    glDrawArrays(GL_TRIANGLES, 0, data.quads.size() * 6);
}
