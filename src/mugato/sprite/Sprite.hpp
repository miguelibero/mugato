

#ifndef __mugato__Sprite__
#define __mugato__Sprite__

#include <mugato/sprite/SpriteAnimation.hpp>
#include <map>

namespace gorn {
    class RenderQueue;
}

namespace mugato {

    class Sprite
    {
    public:
        typedef SpriteAnimation Animation;
        typedef SpriteAtlasRegion Region;
        static const char* kDefaultAnimation;
        static const char* kDefaultFrame;
    private:
        std::map<std::string, Animation> _anims;
        std::string _currentAnim;
    public:
        Sprite();
        Sprite(const Animation& anim);
        Sprite(const std::shared_ptr<gorn::Material>& material);
        Sprite(const std::shared_ptr<gorn::Material>& material,
            const Region& region);

        void setAnimation(const std::string& name, const Animation& anim);
        Animation& setAnimation(const std::string& name);

        void play(const std::string& name);

        void update(double dt);
        void render(gorn::RenderQueue& queue) const;
    };

}

#endif
