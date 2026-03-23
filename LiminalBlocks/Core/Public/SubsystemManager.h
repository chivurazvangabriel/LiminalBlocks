//
// Created by minec on 3/16/2026.
//

#ifndef OOP_SUBSYSTEMMANAGER_H
#define OOP_SUBSYSTEMMANAGER_H
#include <vector>

#include "BaseSubsystem.h"


class SubsystemManager {
    std::vector<BaseSubsystem*> subsystems;

    public:

    SubsystemManager();

    template<typename T>
    T* GetSubsystem() const;

    template<typename T>
    T* RegisterNewSubsystem();

    // statics
    static SubsystemManager* Get();
};

template<typename T>
T * SubsystemManager::GetSubsystem() const {
    for (auto subsystem : subsystems) {
        if (auto ss = dynamic_cast<T*>(subsystem)) {
            return ss;
        }
    }

    return nullptr;
}

template<typename T>
T* SubsystemManager::RegisterNewSubsystem() {
    auto subsystem = new T();
    subsystems.push_back(subsystem);
    return subsystem;
}



#endif //OOP_SUBSYSTEMMANAGER_H
