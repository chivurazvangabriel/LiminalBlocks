//
// Created by minec on 3/11/2026.
//

#ifndef OOP_TICKMANAGER_H
#define OOP_TICKMANAGER_H
#include "BaseSubsystem.h"


class TickManager : public BaseSubsystem{

    public:

    static float DeltaTime;
    static float Time;
};


#endif //OOP_TICKMANAGER_H