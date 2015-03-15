#ifndef __mugato__Rectangle__
#define __mugato__Rectangle__


#include <glm/glm.hpp>
#include <gorn/render/VertexArray.hpp>

class buffer;

namespace mugato
{
    enum class RectangleMatchType
    {
        Contain,
        Intersect,
        Overlap
    };

    struct RectangleMatch;
    struct RectangleVertices;

    struct Rectangle
    {
        typedef RectangleMatchType MatchType;
        typedef gorn::DrawMode DrawMode;
        glm::vec3 origin;
        glm::vec3 size;
        
        Rectangle(const glm::vec3& origin=glm::vec3(0.0f),
            const glm::vec3& size=glm::vec3(0.0f));
        Rectangle(const glm::vec2& origin, const glm::vec2& size=glm::vec2(0.0f));

        bool contains(const glm::vec3& point) const;
        bool contains(const glm::vec2& point) const;
        bool contains(const Rectangle& other) const;
        bool intersects(const Rectangle& other) const;
        bool overlaps(const Rectangle& other) const;
        bool matches(const Rectangle& other, MatchType type) const;
        bool matches(const RectangleMatch& match) const;
        float area() const;

        bool isTwoDimentional() const;
        glm::vec3 min() const;
        glm::vec3 max() const;

        buffer getVertices(DrawMode mode=DrawMode::Triangles) const;
        static size_t getVertexCount(DrawMode mode=DrawMode::Triangles);
    };

    struct RectangleMatch
    {
        typedef RectangleMatchType Type;
        Rectangle rectangle;
        Type type;

        RectangleMatch(const Rectangle& r, Type t=Type::Overlap);
        bool matches(const Rectangle& other) const;
    };
}

#endif
