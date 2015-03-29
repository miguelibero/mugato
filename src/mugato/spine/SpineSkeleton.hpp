#ifndef __mugato__SpineSkeleton__
#define __mugato__SpineSkeleton__

#include <spine/SkeletonData.h>
#include <memory>
#include <glm/glm.hpp>

namespace gorn {
    class RenderQueue;
}

struct spSkeleton;
struct spAnimationState;

namespace mugato {

    class Rectangle;

    class SpineSkeleton
    {
    private:
        std::shared_ptr<spSkeletonData> _data;
	    spSkeleton* _skeleton;
	    spAnimationState* _state;
    public:
        SpineSkeleton();
        SpineSkeleton(const std::shared_ptr<spSkeletonData>& data);
        ~SpineSkeleton();

        void play(const std::string& anim);

        void update(double dt);
        void render(gorn::RenderQueue& queue) const;

        Rectangle getBoundingBox() const;
        glm::vec2 getSize() const;
    };
}

#endif
