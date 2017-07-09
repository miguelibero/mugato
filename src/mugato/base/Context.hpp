#ifndef __mugato__Context__
#define __mugato__Context__

#include <gorn/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/spine/SpineManager.hpp>
#include <mugato/label/LabelManager.hpp>
#include <mugato/sprite/ParticleManager.hpp>
#include <mugato/model/ModelManager.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Light.hpp>
#include <mugato/scripting/ScriptingContext.hpp>
#include <glm/glm.hpp>

namespace mugato
{
    class EntityStack;

    class Context
    {
    private:
        gorn::Context _gorn;
        SpriteManager _sprites;
        ParticleManager _particles;
        LabelManager _labels;
        SpineManager _skeletons;
        ModelManager _models;
        EntityStack* _scenes;
		LightingSystem _lighting;
        ScriptingContext _scripting;
        std::shared_ptr<Entity> _root;

        double _fixedUpdateInterval;
        double _fixedUpdatesPerSecond;
        bool _touching;

        void fixedUpdate(double dt);
		void doTouch(const glm::vec2& p, Entity::TouchPhase phase);
    public:

        Context();

        void setFixedUpdatesPerSecond(double fps);

        void update(double dt);
        void draw();
        void touch(const glm::vec2& p);
        void touchEnd(const glm::vec2& p);

        const gorn::Context& getGorn() const;
        gorn::Context& getGorn();

        const SpriteManager& getSprites() const;
        SpriteManager& getSprites();

        const LabelManager& getLabels() const;
        LabelManager& getLabels();

        const SpineManager& getSkeletons() const;
        SpineManager& getSkeletons();

		const ParticleManager& getParticles() const;
		ParticleManager& getParticles();

        const ModelManager& getModels() const;
        ModelManager& getModels();

        const EntityStack& getScenes() const;
        EntityStack& getScenes();

		const LightingSystem& getLighting() const;
		LightingSystem& getLighting();

        const ScriptingContext& getScripting() const;
        ScriptingContext& getScripting();

        const Entity& getRoot() const;
        Entity& getRoot();

    };
}

#endif
