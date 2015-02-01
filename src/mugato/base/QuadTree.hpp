#ifndef __mugato__QuadTree__
#define __mugato__QuadTree__

#include <memory>
#include <mugato/base/Rectangle.hpp>
#include <mugato/base/QuadTreeNode.hpp>

namespace mugato
{
    template<typename T>
    class QuadTree : public QuadTreeNode<T>
    {
    public:
        typedef Rectangle Area;

        QuadTree(const Area& area=Area(1.0f, 1.0f), size_t max=1,
            const glm::vec2& divs=glm::vec2(2.0f));
    };

    template<typename T>
    QuadTree<T>::QuadTree(const Area& area, size_t max, const glm::vec2& divs):
    QuadTreeNode<T>(area, nullptr, max, divs)
    {
    }
}

#endif

