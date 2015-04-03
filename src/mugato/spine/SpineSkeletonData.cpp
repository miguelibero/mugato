
#include <mugato/spine/SpineSkeletonData.hpp>
#include <mugato/base/Exception.hpp>
#include <spine/SkeletonData.h>
#include <spine/SkeletonJson.h>
#include <spine/Skeleton.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>

namespace mugato {

    SpineSkeletonData::SpineSkeletonData(
        const std::shared_ptr<SpineAtlas>& atlas, const std::string& file):
    _atlas(atlas), _data(nullptr), _anim(nullptr)
    {
        spSkeletonJson* json = _atlas->createJson();
	    _data = spSkeletonJson_readSkeletonDataFile(json,
                file.c_str());
        spSkeletonJson_dispose(json);
        if(_data == nullptr)
        {
            throw Exception(json->error ? json->error :
                "Error reading skeleton data.");
        }
        _anim = spAnimationStateData_create(_data);
        if(_anim == nullptr)
        {
            throw Exception(json->error ? json->error :
                "Error creating animation state data.");
        }
    }

    SpineSkeletonData::~SpineSkeletonData()
    {
        spSkeletonData_dispose(_data);
    }

    void SpineSkeletonData::setAnimationMix(const std::string& from,
        const std::string& to, float duration)
    {
        spAnimationStateData_setMixByName(_anim,
            from.c_str(), to.c_str(), duration);
    }

    spSkeleton* SpineSkeletonData::createSkeleton() const
    {
        return spSkeleton_create(_data);
    }

    spAnimationState* SpineSkeletonData::createAnimationState() const
    {
        return spAnimationState_create(_anim);
    }

}
