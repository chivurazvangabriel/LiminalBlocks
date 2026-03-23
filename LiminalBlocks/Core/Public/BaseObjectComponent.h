//
// Created by minec on 3/16/2026.
//

#ifndef OOP_BASEOBJECTCOMPONENT_H
#define OOP_BASEOBJECTCOMPONENT_H
#include "Core/Interfaces/Public/ITickable.h"


class Object;

class BaseObjectComponent : public ITickable {
    Object* owner = nullptr;

    public:

    virtual void Begin() = 0;
    void Tick(float DeltaTime) override = 0;

    void RegisterComponent(Object* object);
    Object* GetOwner() const { return owner; }
};


#endif //OOP_BASEOBJECTCOMPONENT_H