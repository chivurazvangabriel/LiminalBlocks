//
// Created by minec on 3/11/2026.
//
#pragma once

#include <vector>

#include "BaseObjectComponent.h"
#include "Core/Interfaces/Public/ITickable.h"
#include "SFML/OpenGL.hpp"
#include "SFML/Graphics/Transform.hpp"
#include "SFML/System/Vector3.hpp"

using sf::Vector3f;

struct Transform {
    Vector3f position;
    Vector3f rotation;
    Vector3f scale;
};

class Object{

    Transform transform;
    Vector3f velocity;
    Vector3f acceleration;
    float damping = 0.2;

    std::vector<BaseObjectComponent*> components;

public:
    virtual ~Object() = default;

    Object();

    void Translate(Vector3f Translation);

    Transform GetTransform();
    void SetTransform(const Transform& transform);

    Vector3f GetVelocity() const;
    void SetVelocity(const Vector3f &_velocity);

    Vector3f GetAcceleration() const;
    void SetAcceleration(const Vector3f &_acceleration);

    Vector3f GetPosition() const;
    void SetPosition(const Vector3f &position);
    Vector3f GetRotation() const;
    void SetRotation(const Vector3f &rotation);
    Vector3f GetScale() const;
    void SetScale(const Vector3f &scale);

    Vector3f GetForwardDirection() const;
    Vector3f GetRightDirection() const;
    Vector3f GetUpDirection() const;

    template<typename T>
    T* AddComponent();

    template<typename T>
    T* GetComponent() const;

    void ExecutePhysicsTick(float dt);
    void TickComponents(float dt);

    virtual void Begin();
};

template<typename T>
T * Object::AddComponent() {
    T * component = new T();
    components.push_back(component);
    if (auto cast_comp = dynamic_cast<BaseObjectComponent*>(component)) {
        cast_comp->RegisterComponent(this);
    }
    return component;
}

template<typename T>
T * Object::GetComponent() const {
    for (auto& comp : components) {
        if (auto& tcomp = dynamic_cast<T*> (comp)) {
            return tcomp;
        }
    }
    return nullptr;
}
