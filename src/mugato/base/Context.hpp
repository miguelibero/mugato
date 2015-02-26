#ifndef __mugato__Context__
#define __mugato__Context__

#include <gorn/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/label/LabelManager.hpp>
#include <mugato/scene/EntityStack.hpp>
#include <glm/glm.hpp>

namespace mugato
{
    class Context
    {
    private:
        gorn::Context _gorn;
        SpriteManager _sprites;
        LabelManager _labels;
        EntityStack _scenes;
    public:

        Context();

        void update(double dt);
        void render();

        void setScreenSize(const glm::vec2& size);

        const gorn::Context& getGorn() const;
        gorn::Context& getGorn();

        const SpriteManager& getSprites() const;
        SpriteManager& getSprites();

        const LabelManager& getLabels() const;
        LabelManager& getLabels();

        const EntityStack& getScenes() const;
        EntityStack& getScenes();
    };
}

#endif
