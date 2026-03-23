//
// Created by minec on 3/16/2026.
//

#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include "Camera.h"
#include "Object.h"
#include "Core/Interfaces/Public/ITickable.h"


class Player : public Object, public ITickable {
    Camera* camera = nullptr;

    public:
    void Begin() override;
    void Tick(float deltaTime) override;
    void AssignCamera(Camera* _camera);
};


#endif //OOP_PLAYER_H