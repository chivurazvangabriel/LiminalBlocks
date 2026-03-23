//
// Created by minec on 3/12/2026.
//

#ifndef OOP_BLOCKRENDERE_H
#define OOP_BLOCKRENDERE_H
#include <fstream>
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>               // 2. Then SFML

#include "BaseSubsystem.h"
#include "World.h"

#include "Block.h"
#include "Core/Interfaces/Public/ITickable.h"
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"
#include "Core/Statics/ShaderStatics.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::ivec3 worldPosition = glm::ivec3(0);
    Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texCoord) : position(_position), normal(_normal), texCoord(_texCoord) {}
    Vertex(glm::vec3 _position, glm::vec3 _normal) : position(_position), normal(_normal), texCoord(glm::vec2(0,0)) {}
};

struct Face {
    Vertex v1, v2, v3;
    Face(Vertex _v1, Vertex _v2, Vertex _v3) : v1(_v1), v2(_v2), v3(_v3) {}
    Face(glm::vec3 v1p, glm::vec3 v2p, glm::vec3 v3p, glm::vec3 normal) : v1(v1p, normal), v2(v2p, normal), v3(v3p, normal) {}
};

struct Quad {
    Face f1, f2;
    Quad(glm::vec3 v1p, glm::vec3 v2p, glm::vec3 v3p, glm::vec3 v4p, glm::vec3 normal) : f1(v1p, v2p, v3p, normal), f2(v2p, v4p, v3p, normal) {
        f1.v1.texCoord = {0, 1};
        f1.v2.texCoord = {1, 1};
        f1.v3.texCoord = {0, 0};
        f2.v1.texCoord = {1, 1};
        f2.v2.texCoord = {1, 0};
        f2.v3.texCoord = {0, 0};
    }
    static Quad Translate(const Quad& quad, glm::vec3 translation) {
        Quad q(quad);
        q.f1.v1.position += translation;
        q.f1.v2.position += translation;
        q.f1.v3.position += translation;
        q.f2.v1.position += translation;
        q.f2.v2.position += translation;
        q.f2.v3.position += translation;
        return q;
    }

    static Quad ScaleUVs(const Quad& quad, int TextureAtlasSize, int TextureAtlasID) {
        Quad q(quad);
        float step = 1 / (float)TextureAtlasSize;
        float U1 = static_cast<float>(TextureAtlasID % TextureAtlasSize) * step, U2 = U1 + step;
        float V1 = static_cast<float>(TextureAtlasID / TextureAtlasSize) * step, V2 = V1 + step;
        q.f1.v1.texCoord = {U1, V2};
        q.f1.v2.texCoord = {U2, V2};
        q.f1.v3.texCoord = {U1, V1};
        q.f2.v1.texCoord = {U2, V2};
        q.f2.v2.texCoord = {U2, V1};
        q.f2.v3.texCoord = {U1, V1};
        return q;
    }

    void SetWorldPosition(glm::ivec3 worldPosition) {
        f1.v1.worldPosition = worldPosition;
        f1.v2.worldPosition = worldPosition;
        f1.v3.worldPosition = worldPosition;
        f2.v1.worldPosition = worldPosition;
        f2.v2.worldPosition = worldPosition;
        f2.v3.worldPosition = worldPosition;
    }
};

#define vp vertexPositions
#define nor normals

namespace BlockFaceData {
    static glm::vec3 normals[] = {
        {1,0,0},
        {-1,0,0},
        {0,1,0},
        {0,-1,0},
        {0,0,1},
        {0,0,-1},
    };
    /*
    static glm::vec3 vertexPositions[] = {
        // Bottom
        {-.5f, -.5f, .5f},
        {.5f, -.5f, .5f},
        {-.5f, -.5f, -.5f},
        {.5f, -.5f, -.5f},

        // Top
        {-.5f, .5f, .5f},
        {.5f, .5f, .5f},
        {-.5f, .5f, -.5f},
        {.5f, .5f, -.5f},
    };*/

    static glm::vec3 vertexPositions[] = {
        // Bottom (y = 0)
        {0.0f, 0.0f, 1.0f}, // 0
        {1.0f, 0.0f, 1.0f}, // 1
        {0.0f, 0.0f, 0.0f}, // 2
        {1.0f, 0.0f, 0.0f}, // 3

        // Top (y = 1)
        {0.0f, 1.0f, 1.0f}, // 4
        {1.0f, 1.0f, 1.0f}, // 5
        {0.0f, 1.0f, 0.0f}, // 6
        {1.0f, 1.0f, 0.0f}, // 7
    };
    static Quad quad_bottom = Quad(vp[0], vp[2], vp[1], vp[3], nor[3]);
    static Quad quad_top = Quad(vp[4], vp[5], vp[6], vp[7], nor[2]);
    static Quad quad_left = Quad(vp[4], vp[6], vp[0], vp[2], nor[1]);
    static Quad quad_right = Quad(vp[1], vp[3], vp[5], vp[7], nor[0]);
    static Quad quad_front = Quad(vp[6], vp[7], vp[2], vp[3], nor[5]);
    static Quad quad_back = Quad(vp[4], vp[0], vp[5], vp[1], nor[4]);
}

struct ChunkRenderingData {
    bool dirty = true;
    unsigned int VAO, VBO, EBO;
    std::vector<Quad> quads;
};

enum RelativeBlockVisibility : int{
    TOP = 2, BOTTOM = 1, LEFT = 4, RIGHT = 8, FRONT = 16, BACK = 32
};

class BlockRenderer : public ITickable, public BaseSubsystem {
    std::map<Chunk*, ChunkRenderingData> chunkRenderingData;

public:

    static std::string loadShaderSource(std::string filename) {
        return ShaderStatics::loadShaderSource(filename);
    }

    static unsigned int createShaderProgram(std::string vertPath, std::string fragPath) {
        return ShaderStatics::createShaderProgram(vertPath, fragPath);
    }

    static unsigned int shaderProgram;

    void CreateChunkRenderingData(Chunk* chunk, World* world);

    Block* highlightedBlock;
    World* world;

    void Init(World* world);

    void MakeChunkDirty(Chunk* chunk);
    void MakeChunkDirtyAfterBlockChange(Chunk* chunk, Block* block);
    static unsigned int TextureAtlasID;
    void LoadTextures();
    void HighlightBlock(Block* block);
    virtual void Tick(float deltaTime) override;
    void DrawChunk(Chunk* chunk, World* world);
};


#endif //OOP_BLOCKRENDERE_H