#ifndef __mugato__OcTree__
#define __mugato__OcTree__

#include <memory>
#include <mugato/base/Rectangle.hpp>
#include <mugato/base/OcTreeNode.hpp>

namespace mugato
{
    template<typename T>
    class OcTree : public OcTreeNode<T>
    {
    public:
        OcTree(const Rectangle& area=Rectangle(1.0f, 1.0f), size_t max=1,
            const glm::vec3& divs=glm::vec3(2.0f));
    };

    template<typename T>
    OcTree<T>::OcTree(const Rectangle& area, size_t max, const glm::vec3& divs):
    OcTreeNode<T>(area, nullptr, max, divs)
    {
    }
}

#endif

