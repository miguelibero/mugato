#ifndef __mugato__OcTree__
#define __mugato__OcTree__

#include <mugato/base/OcTreeNode.hpp>

namespace mugato
{
    template<typename T>
    class OcTree : public OcTreeNode<T>
    {
    public:
        OcTree(const gorn::Rect& area=gorn::Rect(), size_t max=1,
            const glm::vec3& divs=glm::vec3(2.0f));
    };

    template<typename T>
    OcTree<T>::OcTree(const gorn::Rect& area, size_t max, const glm::vec3& divs):
    OcTreeNode<T>(area, nullptr, max, divs)
    {
    }
}

#endif

