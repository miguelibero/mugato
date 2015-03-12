#ifndef __mugato__Rectangle__
#define __mugato__Rectangle__


#include <glm/glm.hpp>
#include <gorn/render/VertexArray.hpp>

class buffer;

namespace mugato
{
    struct Rectangle
    {
        typedef gorn::DrawMode DrawMode;

        glm::vec2 origin;
        glm::vec2 size;
        
        Rectangle(const glm::vec2& origin, const glm::vec2& size);
        Rectangle(float x, float y, float width, float height);
        Rectangle(const glm::vec2& origin);
        Rectangle(float x=0.0f, float y=0.0f);

        bool contains(const glm::vec2& point) const;
        bool contains(const Rectangle& other) const;
        bool intersects(const Rectangle& other) const;
        bool matches(const Rectangle& other, bool contained) const;
        float area() const;

        glm::vec2 min() const;
        glm::vec2 max() const;

        buffer getVertices(DrawMode mode=DrawMode::Triangles) const;
    };
}

#endif
