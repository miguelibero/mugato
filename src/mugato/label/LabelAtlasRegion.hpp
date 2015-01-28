

#ifndef __mugato__LabelAtlasRegion__
#define __mugato__LabelAtlasRegion__

#include <mugato/sprite/SpriteAtlasRegion.hpp>

namespace mugato {

    class LabelAtlasRegion : public SpriteAtlasRegion
    {

    private:
        float _advance;

    public:
        LabelAtlasRegion();

        float getAdvance() const;
        void setAdvance(float value);
    };

}

#endif
