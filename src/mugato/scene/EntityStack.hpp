#ifndef __mugato__EntityStack__
#define __mugato__EntityStack__

#include <mugato/scene/Component.hpp>
#include <deque>
#include <memory>

namespace mugato
{
    class Context;

    class EntityStack : public Component
    {
    public:
        typedef std::deque<std::shared_ptr<Entity>> Stack;

    private:
        Stack _stack;
        Context* _ctx;
    public:

        EntityStack();

        void setContext(Context& ctx);

        void update(double dt) override;

        void render(gorn::RenderQueue& queue) override;
        
        Entity& push(std::shared_ptr<Entity> entity=nullptr);

        Entity& get();

        void pop();

        Entity& set(std::shared_ptr<Entity> entity=nullptr);
    };
}

#endif
