#include <mugato/base/Rectangle.hpp>
#include <buffer.hpp>

namespace mugato
{
    Rectangle::Rectangle(const glm::vec3& origin, const glm::vec3& size):
    origin(origin), size(size)
    {
    }

    Rectangle::Rectangle(const glm::vec2& origin, const glm::vec2& size):
    origin(origin.x, origin.y, 0.0f), size(size.x, size.y, 0.0f)
    {
    }

    glm::vec3 Rectangle::min() const
    {
        return origin;
    }

    glm::vec3 Rectangle::max() const
    {
        return origin+size;
    }

    bool Rectangle::isTwoDimentional() const
    {
        return origin.z == 0.0f && size.z == 0.0f;
    }

    bool Rectangle::contains(const glm::vec2& point) const
    {
        auto tmin = min();
        auto tmax = max();
        return point.x >= tmin.x && point.x <= tmax.x
            && point.y >= tmin.y && point.y <= tmax.y;
    }

    bool Rectangle::contains(const glm::vec3& point) const
    {
        auto tmin = min();
        auto tmax = max();
        return point.x >= tmin.x && point.x <= tmax.x
            && point.y >= tmin.y && point.y <= tmax.y
            && point.z >= tmin.z && point.z <= tmax.z;
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
            tmax.x < omin.x ||
            omax.x < tmin.x ||
            tmax.y < omin.y ||
            omax.y < tmin.y ||
            tmax.z < omin.z ||
            omax.z < tmin.z
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
        bool flat = isTwoDimentional();
        switch(mode)
        {
        case DrawMode::Quads:
        {
            if(flat)
            {
                return buffer({
                    tmin.x, tmin.y,
                    tmax.x, tmin.y,
                    tmax.x, tmax.y,
                    tmin.x, tmax.y
                });
            }
            else
            {
                // TODO
                return buffer();
            }
            break;
        }
        case DrawMode::Triangles:
        {
            if(flat)
            {
                return buffer({
                    tmin.x, tmin.y,
                    tmax.x, tmin.y,
                    tmin.x, tmax.y,
                    tmax.x, tmax.y,
                    tmin.x, tmax.y,
                    tmax.x, tmin.y
                });
            }
            else
            {
                // TODO
                return buffer();
            }
            break;
        }
        case DrawMode::Lines:
        {
            if(flat)
            {
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
            else
            {
                // TODO
                return buffer();
            }
            break;
        }
        default:
            return buffer();
            break;
        }
    }
}
