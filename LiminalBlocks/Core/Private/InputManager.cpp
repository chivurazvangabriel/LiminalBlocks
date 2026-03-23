//
// Created by minec on 3/11/2026.
//

#include "../Public/InputManager.h"

#include <iostream>
#include <ostream>

void InputManager::FireEvent(KeyEvent keyEvent) {
    for (const auto& event : eventMap[keyEvent]) {
        event();
    }
}

bool InputManager::IsKeyDown(Key key) {
    for (const auto& downkey : CurrentlyDownKeys) {
        if (downkey == key) {
            return true;
        }
    }
    return false;
}

void InputManager::BindEventToKey(const KeyEvent& keyEvent, Event event) {
    eventMap[keyEvent].push_back(event);
}

sf::Vector2f InputManager::GetMouseDelta() const {
    return MouseDelta;
}

void InputManager::Tick(float deltaTime) {
    static std::vector<Key> CheckedKeys = {
        Key::A, Key::W, Key::S, Key::D, Key::E, Key::Q, Key::R, Key::Z, Key::X, Key::Space, Key::LControl, Key::LShift, Key::LAlt,
    };

    for (const auto& key : CheckedKeys) {
        if (sf::Keyboard::isKeyPressed(key)) {
            KeyEvent keyEvent;
            keyEvent.key = key;
            if (!IsKeyDown(key)) {
                keyEvent.type = PRESSED;
            }
            else {
                keyEvent.type = DOWN;
            }
            FireEvent(keyEvent);
        }
    }

    for (const auto& key : CurrentlyDownKeys) {
        if (!sf::Keyboard::isKeyPressed(key)) {
            KeyEvent keyEvent(key, UP);
            FireEvent(keyEvent);
        }
    }

    CurrentlyDownKeys.clear();

    for (const auto& key : CheckedKeys) {
        if (sf::Keyboard::isKeyPressed(key)) {
            CurrentlyDownKeys.push_back(key);
        }
    }
}
