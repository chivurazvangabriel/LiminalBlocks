//
// Created by minec on 3/11/2026.
//

#include "Core/Public/Object.h"

#include <cmath>

Object::Object() {

}

void Object::Translate(sf::Vector3f Translation) {
    SetPosition(Translation + GetPosition());
}

Transform Object::GetTransform() {
    return transform;
}

void Object::SetTransform(const Transform &_transform) {
    transform = _transform;
}

Vector3f Object::GetVelocity() const {
    return velocity;
}

void Object::SetVelocity(const Vector3f &_velocity) {
    velocity = _velocity;
}

Vector3f Object::GetAcceleration() const {
    return acceleration;
}

void Object::SetAcceleration(const Vector3f &_acceleration) {
    acceleration = _acceleration;
}

Vector3f Object::GetPosition() const{
    return transform.position;
}

void Object::SetPosition(const Vector3f &position) {
    transform.position = position;
}

Vector3f Object::GetRotation() const{
    return transform.rotation;
}

void Object::SetRotation(const Vector3f &rotation) {
    transform.rotation = rotation;
}

Vector3f Object::GetScale() const{
    return transform.scale;
}

void Object::SetScale(const Vector3f &scale) {
    transform.scale = scale;
}

Vector3f safe_normalize(const Vector3f &vec) {
    if (vec.lengthSquared() != 0) return vec.normalized();
    return vec;
}

Vector3f Object::GetForwardDirection() const {
    float yawRad = GetRotation().y * (3.14159f / 180.0f);
    float pitchRad = GetRotation().x * (3.14159f / 180.0f);

    sf::Vector3f direction;
    direction.x = std::cos(yawRad) * cos(pitchRad);
    direction.y = sin(pitchRad);
    direction.z = sin(yawRad) * cos(pitchRad);

    return safe_normalize(direction);
}

Vector3f Object::GetRightDirection() const {
    return safe_normalize(GetForwardDirection().cross({0,1,0}));
}

Vector3f Object::GetUpDirection() const {
    return safe_normalize(GetForwardDirection().cross({1,0,0}));
}

void Object::ExecutePhysicsTick(float dt) {
    SetPosition(GetPosition() + GetVelocity() * dt);
    SetVelocity(GetVelocity() + GetAcceleration() * dt);
}

void Object::TickComponents(float dt) {
    for (auto comp : components) {
        if (auto tickable = dynamic_cast<ITickable*>(comp)) {
            tickable->Tick(dt);
        }
    }
}

void Object::Begin() {
}
