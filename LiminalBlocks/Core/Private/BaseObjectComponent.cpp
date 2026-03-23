//
// Created by minec on 3/16/2026.
//

#include "../Public/BaseObjectComponent.h"

void BaseObjectComponent::RegisterComponent(Object *object) {
    owner = object;
}
