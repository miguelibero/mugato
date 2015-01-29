
#include <mugato/label/FontAtlasRegion.hpp>

namespace mugato {

    FontAtlasRegion::FontAtlasRegion():
    _advance(0.0f)
    {
    }

    float FontAtlasRegion::getAdvance() const
    {
        auto sizex = getOriginalSize().x;
        if(sizex < _advance)
        {
            return _advance;
        }
        else
        {
            return sizex;
        }
    }

    void FontAtlasRegion::setAdvance(float value)
    {
        _advance = value;
    }
}
