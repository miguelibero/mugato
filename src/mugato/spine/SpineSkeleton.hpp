#ifndef __mugato__SpineSkeleton__
#define __mugato__SpineSkeleton__

#include <memory>
#include <glm/glm.hpp>
#include <gorn/render/Material.hpp>
#include <mugato/spine/SpineSkeletonData.hpp>

namespace gorn {
    class RenderQueue;
    class Rect;
}

struct spSkeleton;
struct spAnimationState;

namespace mugato {

    class SpineSkeleton
    {
    public:
        typedef SpineSkeletonData Data;
    private:
        std::shared_ptr<Data> _data;
	    spSkeleton* _skeleton;
        spAnimationState* _anim;
        std::shared_ptr<gorn::Material> _slotsMaterial;
        std::shared_ptr<gorn::Material> _bonesMaterial;
        void cleanup();

        void renderSkeleton(gorn::RenderQueue& queue) const;
        void renderSlots(gorn::RenderQueue& queue) const;
        void renderBones(gorn::RenderQueue& queue) const;
    public:
        SpineSkeleton();
        SpineSkeleton(const std::shared_ptr<Data>& data);
        ~SpineSkeleton();

        void setSlotsMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setBonesMaterial(const std::shared_ptr<gorn::Material>& mat);

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

        gorn::Rect getBoundingBox() const;
        glm::vec2 getSize() const;
    };
}

#endif
