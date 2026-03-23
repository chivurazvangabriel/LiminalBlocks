//
// Created by minec on 3/16/2026.
//

#include "../Public/SubsystemManager.h"

SubsystemManager::SubsystemManager() {

}

SubsystemManager * SubsystemManager::Get() {
    static SubsystemManager* instance = new SubsystemManager();
    return instance;
}
