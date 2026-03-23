//
// Created by minec on 3/16/2026.
//

#ifndef OOP_COLLIDERCOMPONENT_H
#define OOP_COLLIDERCOMPONENT_H
#include "BaseObjectComponent.h"
#include "Object.h"
#include "SFML/System/Vector3.hpp"
using sf::Vector3f;

struct BoxTraceData {
    float Time;
    Vector3f Normal;
};

struct  Box {
    sf::Vector3f center, extent;

    bool DoesOverlap(const Box& other) const {
         return std::abs(center.x - other.center.x) < (extent.x + other.extent.x) &&
                std::abs(center.y - other.center.y) < (extent.y + other.extent.y) &&
                std::abs(center.z - other.center.z) < (extent.z + other.extent.z);
    }

    float GetOverlapOnAxis(const Box& other, sf::Vector3f axis) const {
        if (axis.x != 0) return CalculateOverlap(center.x, extent.x, other.center.x, other.extent.x);
        if (axis.y != 0) return CalculateOverlap(center.y, extent.y, other.center.y, other.extent.y);
        if (axis.z != 0) return CalculateOverlap(center.z, extent.z, other.center.z, other.extent.z);
        return 0;
    }

    static BoxTraceData SweptAABB(Box mover, Vector3f velocity, Box target) {
        BoxTraceData data;
        Vector3f normal = {0,0,0};

        // 1. Create the "Minkowski" expanded target box
        Vector3f bMin = target.center - target.extent - mover.extent;
        Vector3f bMax = target.center + target.extent + mover.extent;

        // 2. Calculate entry/exit times for each axis
        float tXEntry, tXExit, tYEntry, tYExit, tZEntry, tZExit;

        // Standard AABB Ray-box intersection logic
        // (Note: Handle division by zero if velocity component is 0 in production)
        tXEntry = (bMin.x - mover.center.x) / velocity.x;
        tXExit  = (bMax.x - mover.center.x) / velocity.x;
        if (tXEntry > tXExit) std::swap(tXEntry, tXExit);

        tYEntry = (bMin.y - mover.center.y) / velocity.y;
        tYExit  = (bMax.y - mover.center.y) / velocity.y;
        if (tYEntry > tYExit) std::swap(tYEntry, tYExit);

        tZEntry = (bMin.z - mover.center.z) / velocity.z;
        tZExit  = (bMax.z - mover.center.z) / velocity.z;
        if (tZEntry > tZExit) std::swap(tZEntry, tZExit);

        // 3. Find the latest entry and earliest exit
        float entryTime = std::max(tXEntry, std::max(tYEntry, tZEntry));
        float exitTime  = std::min(tXExit, std::min(tYExit, tZExit));

        // 4. Check if a hit actually occurred
        if (entryTime > exitTime || (tXEntry < 0 && tYEntry < 0 && tZEntry < 0) || tXEntry > 1 || tYEntry > 1 || tZEntry > 1) {
            data.Normal = normal;
            data.Time = 1; // No collision this frame
            return data;
        }

        // 5. Calculate the hit normal (which side did we hit?)
        if (tXEntry > tYEntry && tXEntry > tZEntry) {
            normal = Vector3f(velocity.x < 0 ? 1 : -1, 0, 0);
        } else if (tYEntry > tXEntry && tYEntry > tZEntry) {
            normal = Vector3f(0, velocity.y < 0 ? 1 : -1, 0);
        } else {
            normal = Vector3f(0, 0, velocity.z < 0 ? 1 : -1);
        }

        data.Normal = normal;
        data.Time = entryTime;
        return data;
    }

private:
    float CalculateOverlap(float c1, float e1, float c2, float e2) const {
        float dist = c1 - c2;
        float overlap = (e1 + e2) - std::abs(dist);
        return (dist > 0) ? overlap : -overlap;
    }
};

class ColliderComponent : public BaseObjectComponent {
    sf::Vector3f ColliderExtents = sf::Vector3f(0.3f, 0.9f, 0.3f);
public:

    void Begin() override;
    void Tick(float deltaTime) override;

    Box GetBox() const {return {GetOwner()->GetPosition(), ColliderExtents};}
};


#endif //OOP_COLLIDERCOMPONENT_H