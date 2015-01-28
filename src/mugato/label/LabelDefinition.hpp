

#ifndef __mugato__LabelDefinition__
#define __mugato__LabelDefinition__

#include <string>

namespace mugato {


    class LabelDefinition
    {
    private:
        std::string _material;
        std::string _atlas;
    public:
        LabelDefinition();

        LabelDefinition& withAtlas(const std::string& atlas);

        const std::string& getAtlas() const;
    };

}

#endif
