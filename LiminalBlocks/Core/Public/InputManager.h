//
// Created by minec on 3/11/2026.
//

#ifndef OOP_INPUTMANAGER_H
#define OOP_INPUTMANAGER_H
#include "SFML/Window/Keyboard.hpp"
#include "functional"
#include "../Interfaces/Public/ITickable.h"
#include "vector"
#include "map"
#include <algorithm>

#include "BaseSubsystem.h"
#include "unordered_map"
#include "SFML/System/Vector2.hpp"

typedef sf::Keyboard::Key Key;

enum KeyEventType {
    PRESSED,
    DOWN,
    UP
};

struct KeyEvent {
    Key key;
    KeyEventType type;
    KeyEvent(Key _key, KeyEventType _type) : key(_key), type(_type) {}
    KeyEvent() : key(), type(KeyEventType::PRESSED) {}

    bool operator==(const KeyEvent &a) const {
        return a.key == key && a.type == type;
    }
};

template <>
    struct std::hash<KeyEvent> {
    size_t operator()(const KeyEvent& k) const noexcept {
        // Use the internal value of the SFML key as the hash
        return std::hash<int>()(static_cast<int>(k.key) * (k.type * 100 + 1001));
    }
};

typedef std::function<void()> Event;
typedef std::vector<Event> EventList;
typedef std::unordered_map<KeyEvent, EventList> EventMap;

class InputManager : public BaseSubsystem, public ITickable {
    EventMap eventMap;
    std::vector<Key> CurrentlyDownKeys;

    sf::Vector2f MouseDelta;

protected:

    void FireEvent(KeyEvent keyEvent);

public:

    bool IsKeyDown(Key key);
    void BindEventToKey(const KeyEvent& keyEvent, Event event);
    sf::Vector2f GetMouseDelta() const;
    void Tick(float deltaTime) override;
};


#endif //OOP_INPUTMANAGER_H