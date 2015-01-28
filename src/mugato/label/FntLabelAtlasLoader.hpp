
#ifndef __mugato__FntLabelAtlasLoader__

#include <gorn/asset/AssetLoader.hpp>

namespace mugato {

    class LabelAtlas;

    class FntLabelAtlasLoader : public gorn::AssetLoader<LabelAtlas>
    {
    public:
        FntLabelAtlasLoader();
        bool validate(const gorn::Data& data) const;
        LabelAtlas load(gorn::Data&& data) const;
          
    };
}

#endif
