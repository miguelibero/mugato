#ifndef __mugato__Context__
#define __mugato__Context__

#include <gorn/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/label/LabelManager.hpp>

namespace mugato
{
    class Context
    {
    private:
        gorn::Context _gorn;
        SpriteManager _sprites;
        LabelManager  _labels;
    public:

        Context();

        const gorn::Context& getGorn() const;
        gorn::Context& getGorn();

        const SpriteManager& getSprites() const;
        SpriteManager& getSprites();

        const LabelManager& getLabels() const;
        LabelManager& getLabels();
    };
}

#endif
