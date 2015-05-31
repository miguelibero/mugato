

#ifndef __mugato__Enums__
#define __mugato__Enums__

#include <glm/glm.hpp>

namespace mugato
{   
    enum class Alignment
    {
        TopLeft,
        Top,
        TopRight,
        Left,
        Center,
        Right,
        BottomLeft,
        Bottom,
        BottomRight   
    };

    glm::vec3 alignPosition(Alignment align,
        const glm::vec2& outside,
        const glm::vec2& inside);

    glm::vec3 alignPosition(Alignment align,
        const glm::vec3& outside,
        const glm::vec3& inside);
}

#endif
