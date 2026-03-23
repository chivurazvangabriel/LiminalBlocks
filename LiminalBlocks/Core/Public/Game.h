//
// Created by minec on 3/16/2026.
//

#ifndef OOP_GAME_H
#define OOP_GAME_H
#include "Core/Interfaces/Public/ITickable.h"


class Game : public ITickable {
    Game();
public:
    void CreateGame();
    void Tick(float deltaTime) override;

    static Game* Get();
};


#endif //OOP_GAME_H