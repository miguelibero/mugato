#ifndef __mugato__SpineSkeletonDefinition__
#define __mugato__SpineSkeletonDefinition__

#include <string>

namespace mugato {


    class SpineSkeletonDefinition
    {
    private:
        std::string _dataFile;
        std::string _atlasFile;
    public:
        SpineSkeletonDefinition();

        SpineSkeletonDefinition& withDataFile(const std::string& name);
        SpineSkeletonDefinition& withAtlasFile(const std::string& name);

        const std::string& getDataFile() const;
        const std::string& getAtlasFile() const;
    };
}

#endif
