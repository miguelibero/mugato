#ifndef __mugato__SpineSkeleton__
#define __mugato__SpineSkeleton__

#include <memory>
#include <glm/glm.hpp>
#include <mugato/spine/SpineSkeletonData.hpp>

namespace gorn {
    class RenderQueue;
}

struct spSkeleton;
struct spAnimationState;

namespace mugato {

    class Rectangle;

    class SpineSkeleton
    {
    public:
        typedef SpineSkeletonData Data;
    private:
        std::shared_ptr<Data> _data;
	    spSkeleton* _skeleton;
        spAnimationState* _anim;
        
        void cleanup();
    public:
        SpineSkeleton();
        SpineSkeleton(const std::shared_ptr<Data>& data);
        ~SpineSkeleton();

        SpineSkeleton(const SpineSkeleton& other);
        SpineSkeleton& operator=(const SpineSkeleton& other);

        void setAnimation(const std::string& name, int track, bool loop=true);
        void setAnimation(const std::string& name, bool loop=true);
        void addAnimation(const std::string& name, int track,
            bool loop=true, float delay=0.0f);
        void addAnimation(const std::string& name,
            bool loop=true, float delay=0.0f);

        void update(double dt);
        void render(gorn::RenderQueue& queue) const;

        Rectangle getBoundingBox() const;
        glm::vec2 getSize() const;
    };
}

#endif
