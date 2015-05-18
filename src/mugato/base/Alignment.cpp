
#include <mugato/base/Alignment.hpp>

namespace mugato
{   
    glm::vec3 alignPosition(Alignment align,
        const glm::vec2& outside,
        const glm::vec2& inside)
    {
        return alignPosition(align,
            glm::vec3(outside, 0.0f),
            glm::vec3(inside, 0.0f));
    }

    glm::vec3 alignPosition(Alignment align,
        const glm::vec3& outside,
        const glm::vec3& inside)
    {
        switch(align)
        {
        case Alignment::TopLeft:
            return glm::vec3(
                0.0f,
                (outside.y-inside.y),
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::Top:
            return glm::vec3(
                (outside.x-inside.x)*0.5f,
                (outside.y-inside.y),
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::TopRight:
            return glm::vec3(
                (outside.x-inside.x),
                (outside.y-inside.y),
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::Left:
            return glm::vec3(
                0.0f,
                (outside.y-inside.y)*0.5f,
                (outside.z-inside.z)*0.5f);
            break;
        default:
        case Alignment::Center:
            return glm::vec3(
                (outside.x-inside.x)*0.5f,
                (outside.y-inside.y)*0.5f,
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::Right:
            return glm::vec3(
                (outside.x-inside.x),
                (outside.y-inside.y)*0.5f,
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::BottomLeft:
            return glm::vec3(
                0.0f,
                0.0f,
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::Bottom:
            return glm::vec3(
                (outside.x-inside.x)*0.5f,
                0.0f,
                (outside.z-inside.z)*0.5f);
            break;
        case Alignment::BottomRight:
            return glm::vec3(
                (outside.x-inside.x),
                0.0f,
                (outside.z-inside.z)*0.5f);
            break;
        }
    }
}
