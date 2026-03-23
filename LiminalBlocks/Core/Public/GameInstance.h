//
// Created by minec on 3/11/2026.
//

#ifndef OOP_GAMEINSTANCE_H
#define OOP_GAMEINSTANCE_H

#include "BaseSubsystem.h"
#include "Object.h"
#include "vector"

class GameInstance : public BaseSubsystem {
    public:

    std::vector<Object*> objects;

    template<typename T>
    T* CreateObject();

    template<typename T>
    bool DestroyObject(T* Obj);
};

template<typename T>
T * GameInstance::CreateObject() {
    Object * obj = new T();
    objects.push_back(obj);
    obj->Begin();
    return static_cast<T*>(obj);
}

template<typename T>
bool GameInstance::DestroyObject(T *Obj) {
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        if (*it == Obj) {
            objects.erase(it);
            delete Obj;
            return true;
        }
    }
    return false;
}


#endif //OOP_GAMEINSTANCE_H
