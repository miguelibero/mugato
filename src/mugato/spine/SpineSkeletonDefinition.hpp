#ifndef __mugato__SpineSkeletonDefinition__
#define __mugato__SpineSkeletonDefinition__

#include <string>
#include <vector>
#include <mugato/spine/SpineAnimationMixDefinition.hpp>

namespace mugato {

    class SpineSkeletonDefinition
    {
    public:
        typedef SpineAnimationMixDefinition AnimationMix;
        typedef std::vector<AnimationMix> AnimationMixes;
    private:
        std::string _dataFile;
        std::string _atlasFile;
        AnimationMixes _mixes;
    public:
        SpineSkeletonDefinition();

        SpineSkeletonDefinition& withDataFile(const std::string& name);
        SpineSkeletonDefinition& withAtlasFile(const std::string& name);
        SpineSkeletonDefinition& withAnimationMix(const AnimationMix& mix);

        const AnimationMixes& getAnimationMixes() const;
        const std::string& getDataFile() const;
        const std::string& getAtlasFile() const;
    };
}

#endif
