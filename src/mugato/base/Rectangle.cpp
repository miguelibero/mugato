#include <mugato/base/Rectangle.hpp>
#include <buffer.hpp>
#include <limits>

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

    bool Rectangle::flat() const
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

    bool Rectangle::overlaps(const Rectangle& other) const
    {
        return contains(other.min()) || contains(other.max());
    }

    bool Rectangle::matches(const Rectangle& other, MatchType type) const
    {
        switch(type)
        {
        case MatchType::Contain:
            return contains(other);
        case MatchType::Overlap:
            return overlaps(other);
        case MatchType::Intersect:
            return intersects(other);
        default:
            return false;
        }
    }

    bool Rectangle::matches(const RectangleMatch& match) const
    {
        return matches(match.rectangle, match.type);
    }

    float Rectangle::area() const
    {
        return size.x*size.y;
    }

    buffer Rectangle::getVertices(DrawMode mode) const
    {
        auto tmin = min();
        auto tmax = max();
        bool tflat = flat();
        switch(mode)
        {
        case DrawMode::Quads:
        {
            if(tflat)
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
            if(tflat)
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
            if(tflat)
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
            }
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

    size_t Rectangle::getVertexCount(DrawMode mode)
    {
        switch(mode)
        {
        case DrawMode::Quads:
            return 4;
        case DrawMode::Triangles:
            return 6;
        case DrawMode::Lines:
            return 8;
        default:
            return 0;
        }
    }

    std::array<glm::vec3,8> Rectangle::corners() const
    {
        return {            
            glm::vec3(origin.x,        origin.y,        origin.z),
            glm::vec3(origin.x+size.x, origin.y,        origin.z),
            glm::vec3(origin.x+size.x, origin.y+size.y, origin.z),
            glm::vec3(origin.x,        origin.y+size.y, origin.z),
            glm::vec3(origin.x,        origin.y,        origin.z+size.z),
            glm::vec3(origin.x+size.x, origin.y,        origin.z+size.z),
            glm::vec3(origin.x+size.x, origin.y+size.y, origin.z+size.z),
            glm::vec3(origin.x,        origin.y+size.y, origin.z+size.z)
        };
    }

    Rectangle Rectangle::operator*(const glm::mat4& t) const
    {
        Rectangle r(*this);
        r *= t;
        return r;
    }

    Rectangle& Rectangle::operator*=(const glm::mat4& t)
    {
        auto cs = corners();
        glm::vec3 max;
        glm::vec3 min;
        bool init = false;    
        for(auto& p : cs)
        {
            p = glm::vec3(t*glm::vec4(p, 1.0f));
            if(init)
            {
                min.x = glm::min(min.x, p.x);
                min.y = glm::min(min.y, p.y);
                min.z = glm::min(min.z, p.z);
                max.x = glm::max(max.x, p.x);
                max.y = glm::max(max.y, p.y);
                max.z = glm::max(max.z, p.z);
            }
            else
            {
                min = p;
                max = p;
                init = true;
            }
        }
        origin = min;
        size = max - min;
        return *this;
    }

    std::vector<Rectangle> Rectangle::divide(const glm::vec3& divs) const
    {
        auto fdivs = glm::round(glm::max(divs, glm::vec3(1.0f)));
        std::vector<Rectangle> parts;
        parts.reserve(fdivs.x*fdivs.y*fdivs.z);
        auto tmin = min();
        auto tmax = max();
        auto tsize = size/fdivs;
        auto p = tmin;

        for(p.z=tmin.z;p.z<tmax.z||p.z==tmin.z;p.z+=tsize.z)
        {
            for(p.y=tmin.y;p.y<tmax.y||p.y==tmin.y;p.y+=tsize.y)
            {
                for(p.x=tmin.x;p.x<tmax.x||p.x==tmin.x;p.x+=tsize.x)
                {
                    parts.push_back(Rectangle(p, tsize));
                    if(tsize.x == 0.0f)
                    {
                        break;
                    }
                }
                if(tsize.y == 0.0f)
                {
                    break;
                }
            }
            if(tsize.z == 0.0f)
            {
                break;
            }
        }

        return parts;
    }

    RectangleMatch::RectangleMatch(const Rectangle& r, Type t):
    rectangle(r), type(t)
    {
    }

    bool RectangleMatch::matches(const Rectangle& other) const
    {
        return rectangle.matches(other, type);
    }
}
