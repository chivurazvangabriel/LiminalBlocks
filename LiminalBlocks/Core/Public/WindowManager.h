//
// Created by minec on 3/16/2026.
//

#ifndef OOP_WINDOWMANAGER_H
#define OOP_WINDOWMANAGER_H
#include "BaseSubsystem.h"
#include "SFML/Window/Window.hpp"


namespace sf {
    class Window;
}

class WindowManager : public BaseSubsystem{
    public:

    sf::Window& InitializeWindow();
    sf::Window window;
};


#endif //OOP_WINDOWMANAGER_H