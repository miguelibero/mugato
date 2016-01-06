#ifndef __mugato__Context__
#define __mugato__Context__

#include <gorn/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/spine/SpineManager.hpp>
#include <mugato/label/LabelManager.hpp>
#include <mugato/scene/Entity.hpp>
#include <glm/glm.hpp>

namespace mugato
{
    class EntityStack;

    class Context
    {
    private:
        gorn::Context _gorn;
        SpriteManager _sprites;
        LabelManager _labels;
        SpineManager _skeletons;
        EntityStack* _scenes;
        std::shared_ptr<Entity> _root;

        double _fixedUpdateInterval;
        double _fixedUpdatesPerSecond;

        void fixedUpdate(double dt);
    public:

        Context();

        void setFixedUpdatesPerSecond(double fps);

        void update(double dt);
        void draw();
        void touch(const glm::vec2& p);
        void touchEnd(const glm::vec2& p);

        void setViewportSize(const glm::vec2& size);

        const gorn::Context& getGorn() const;
        gorn::Context& getGorn();

        const SpriteManager& getSprites() const;
        SpriteManager& getSprites();

        const LabelManager& getLabels() const;
        LabelManager& getLabels();

        const SpineManager& getSkeletons() const;
        SpineManager& getSkeletons();

        const EntityStack& getScenes() const;
        EntityStack& getScenes();

        const Entity& getRoot() const;
        Entity& getRoot();

    };
}

#endif
