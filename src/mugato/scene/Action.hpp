
#ifndef __mugato__Action__
#define __mugato__Action__

namespace mugato {

    class Entity;

    class Action
    {
    public:
        virtual ~Action(){}
        virtual void start(Entity& entity){};
        virtual void update(Entity& entity, float pos){};
        virtual void finish(Entity& entity){};
    };
}

#endif
