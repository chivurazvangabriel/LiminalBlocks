//
// Created by minec on 3/16/2026.
//

#include "../Public/WindowManager.h"

#include "glad/include/glad/glad.h"
#include "SFML/OpenGL.hpp"
#include "SFML/Window/Context.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/Window.hpp"

sf::Window& WindowManager::InitializeWindow() {
    window.create(sf::VideoMode({1600, 900}), "My window");

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {

    }
    else {
        glEnable(GL_DEPTH_TEST);

        window.setFramerateLimit(260);
        window.setMouseCursorVisible(false);
        window.setMouseCursorGrabbed(true);
    }

    return window;
}
