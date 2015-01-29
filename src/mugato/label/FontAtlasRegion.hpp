

#ifndef __mugato__FontAtlasRegion__
#define __mugato__FontAtlasRegion__

#include <mugato/sprite/SpriteAtlasRegion.hpp>

namespace mugato {

    class FontAtlasRegion : public SpriteAtlasRegion
    {

    private:
        float _advance;

    public:
        FontAtlasRegion();

        float getAdvance() const;
        void setAdvance(float value);
    };

}

#endif
