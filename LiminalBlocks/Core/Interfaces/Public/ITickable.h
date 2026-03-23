//
// Created by minec on 3/11/2026.
//

#ifndef OOP_ITICKABLE_H
#define OOP_ITICKABLE_H
#include "BaseInterface.h"


class ITickable : public BaseInterface {
public:
    virtual void Tick(float deltaTime) = 0;
};


#endif //OOP_ITICKABLE_H