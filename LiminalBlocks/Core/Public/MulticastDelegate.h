//
// Created by minec on 3/15/2026.
//

#ifndef OOP_MULTICASTDELEGATE_H
#define OOP_MULTICASTDELEGATE_H
#include <functional>
#include <vector>


template <typename... Args>
class MulticastDelegate {
    std::vector<std::function<void(Args...)>> delegates;
public:

    void Broadcast(Args... args) {
        for (auto& delegate : delegates) {
            delegate(args...);
        }
    }

    template<typename T>
    void Bind(T* instance, void (T::*func)(Args...)) {
        delegates.push_back([instance, func](Args... args) {
            (instance->*func)(args...);
        });
    }


    void Unbind(std::function<void(Args... )> delegate) {
        for (auto it = delegates.begin(); it != delegates.end(); ++it) {
            if (delegate.compare(*it) == 0) {
                delegates.erase(it);
                return;
            }
        }
    }
};


#endif //OOP_MULTICASTDELEGATE_H