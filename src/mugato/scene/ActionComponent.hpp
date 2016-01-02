#ifndef __mugato__ActionComponent__
#define __mugato__ActionComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/scene/Action.hpp>
#include <memory>
#include <vector>

namespace mugato
{
    class ActionState
    {
    private:
        std::unique_ptr<Action> _action;
        float _duration;
        float _position;
        bool _started;
    public:
        ActionState(float duration, std::unique_ptr<Action> action);
        void update(double dt);
        void update(Entity& entity);
        bool finished() const;
    };

    class ActionComponent : public Component
    {
    private:
        std::vector<ActionState> _actions;
        std::weak_ptr<Entity> _entity;

    public:
        void add(float duration, std::unique_ptr<Action> action);
        void clear();

        template<typename A, typename... Args>
        A& add(float duration, Args&&... args);

        void onAddedToEntity(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };

    template<typename A, typename... Args>
    A& ActionComponent::add(float duration, Args&&... args)
    {
        auto ptr = new A(std::forward<Args>(args)...);
        add(duration, std::unique_ptr<Action>(ptr));
        return *ptr;
    }
}

#endif
