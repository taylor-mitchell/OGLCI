#pragma once
#include <string>
#include <glm.hpp>

namespace logger {
    namespace helper {
        using std::to_string;

        template<class T>
        std::string as_string(T& t) {
            return to_string(t);
        }
    }
    template<class T>
    std::string to_string(T& t) {
        return helper::as_string(t);
    }
    template<>
    inline std::string to_string(glm::vec2& t) {
        return "(" + helper::as_string(t.x) + ", " + helper::as_string(t.y) + ")";
    }
}