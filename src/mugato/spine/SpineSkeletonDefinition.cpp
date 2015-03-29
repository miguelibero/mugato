
#include <mugato/spine/SpineSkeletonDefinition.hpp>

namespace mugato {

    SpineSkeletonDefinition::SpineSkeletonDefinition()
    {
    }

    SpineSkeletonDefinition& SpineSkeletonDefinition::
        withDataFile(const std::string& name)
    {
        _dataFile = name;
        return *this;
    }

    SpineSkeletonDefinition& SpineSkeletonDefinition::
        withAtlasFile(const std::string& name)
    {
        _atlasFile = name;
        return *this;
    }

    const std::string& SpineSkeletonDefinition::getDataFile() const
    {
        return _dataFile;
    }

    const std::string& SpineSkeletonDefinition::getAtlasFile() const
    {
        return _atlasFile;
    }
}
