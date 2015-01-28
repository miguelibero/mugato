
#include <mugato/label/LabelDefinition.hpp>

namespace mugato {

    LabelDefinition::LabelDefinition()    
    {
    }

    LabelDefinition& LabelDefinition::withAtlas(const std::string& atlas)
    {
        _atlas = atlas;
        return *this;
    }

    const std::string& LabelDefinition::getAtlas() const
    {
        return _atlas;
    }

}
