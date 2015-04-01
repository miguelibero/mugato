#ifndef __mugato__SpineSkeletonData__
#define __mugato__SpineSkeletonData__

#include <string>
#include <memory>
#include <mugato/spine/SpineAtlas.hpp>

struct spSkeletonData;
struct spSkeleton;
struct spAnimationStateData;
struct spAnimationState;

namespace mugato {

    class SpineSkeletonData
    {
    private:
        std::shared_ptr<SpineAtlas> _atlas;
        spSkeletonData* _data;
        spAnimationStateData* _anim;

    public:
        SpineSkeletonData(const std::shared_ptr<SpineAtlas>& atlas,
            const std::string& file);
        ~SpineSkeletonData();
        SpineSkeletonData(const SpineSkeletonData& other) = delete;

        void setAnimationMix(const std::string& from,
        const std::string& to, float duration);

        spSkeleton* createSkeleton() const;
        spAnimationState* createAnimationState() const;
    };

}

#endif
