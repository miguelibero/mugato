#include <mugato/base/Rectangle.hpp>
#include <buffer.hpp>

namespace mugato
{
    Rectangle::Rectangle(const glm::vec2& origin, const glm::vec2& size):
    origin(origin), size(size)
    {
    }

    Rectangle::Rectangle(float x, float y, float width, float height):
    origin(x, y), size(width, height)
    {
    }
    
    Rectangle::Rectangle(const glm::vec2& origin):
    origin(origin)
    {
    }

    Rectangle::Rectangle(float x, float y):
    origin(x, y)
    {
    }

    glm::vec2 Rectangle::min() const
    {
        return origin;
    }

    glm::vec2 Rectangle::max() const
    {
        return origin+size;
    }

    bool Rectangle::contains(const glm::vec2& point) const
    {
        auto tmin = min();
        auto tmax = max();
        return point.x >= tmin.x && point.x <= tmax.x
            && point.y >= tmin.y && point.y <= tmax.y;
    }

    bool Rectangle::contains(const Rectangle& other) const
    {
        return contains(other.min()) && contains(other.max());
    }

    bool Rectangle::intersects(const Rectangle& other) const
    {
        auto tmin = min();
        auto tmax = max();
        auto omin = other.min();
        auto omax = other.max();
        return !( 
            tmax.x <= omin.x ||
            omax.x <= tmin.x ||
            tmax.y <= omin.y ||
            omax.y <= tmin.y
        );
    }

    bool Rectangle::matches(const Rectangle& other, bool contained) const
    {
        if(contained)
        {
            return contains(other);
        }
        else
        {
            return intersects(other);
        }
    }

    float Rectangle::area() const
    {
        return size.x*size.y;
    }

    buffer Rectangle::getVertices(DrawMode mode) const
    {
        auto tmin = min();
        auto tmax = max();
        
        switch(mode)
        {
        case DrawMode::Quads:
            return buffer({
                tmin.x, tmin.y,
                tmax.x, tmin.y,
                tmax.x, tmax.y,
                tmin.x, tmax.y
            });
            break;
        case DrawMode::Triangles:
            return buffer({
                tmin.x, tmin.y,
                tmax.x, tmin.y,
                tmin.x, tmax.y,
                tmax.x, tmax.y,
                tmin.x, tmax.y,
                tmax.x, tmin.y
            });
            break;
        case DrawMode::Lines:
            return buffer({
                tmin.x, tmin.y,
                tmax.x, tmin.y,
                tmax.x, tmin.y,
                tmax.x, tmax.y,
                tmax.x, tmax.y,
                tmin.x, tmax.y,
                tmin.x, tmax.y,
                tmin.x, tmin.y
            });
            break;
        default:
            return buffer();
            break;
        }
    }
}
