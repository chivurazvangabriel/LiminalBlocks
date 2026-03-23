//
// Created by minec on 3/11/2026.
//

#pragma once

#include "Object.h"
#include "Core/Interfaces/Public/ITickable.h"

class Camera : public Object, public ITickable{

    double FOV = 90;

public:

    Camera();

    virtual void Tick(float deltaTime) override;

};
