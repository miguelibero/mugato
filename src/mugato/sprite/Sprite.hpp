

#ifndef __mugato__Sprite__
#define __mugato__Sprite__

#include <mugato/sprite/SpriteAnimation.hpp>
#include <mugato/sprite/SpriteEnums.hpp>
#include <map>
#include <string>

namespace gorn {
    class RenderQueue;
}

namespace mugato {

    class Sprite
    {
    public:
        typedef SpriteResizeMode ResizeMode;
        typedef SpriteAnimation Animation;
        typedef SpriteAtlasRegion Region;
        static const char* kDefaultAnimation;
        static const char* kDefaultFrame;
    private:
        std::map<std::string, Animation> _anims;
        std::string _currentAnim;
        mutable glm::vec2 _size;
        ResizeMode _resizeMode;
        glm::vec4 _stretchBorders;

        void init();
    public:
        Sprite();
        Sprite(const Animation& anim);
        Sprite(const std::shared_ptr<gorn::Material>& material);
        Sprite(const std::shared_ptr<gorn::Material>& material,
            const Region& region);

        void setMaterial(const std::shared_ptr<gorn::Material>& material);

        void setAnimation(const std::string& name, const Animation& anim);
        Animation& setAnimation(const std::string& name);

        const glm::vec2& getSize() const;
        glm::vec2 getOriginalSize() const;
        void setSize(const glm::vec2& size);
        ResizeMode getResizeMode() const;
        void setResizeMode(ResizeMode mode);
        const glm::vec4& getStretchBorders() const;
        void setStretchBorders(const glm::vec4& borders);

        void play(const std::string& name);

        void update(double dt);
        void render(gorn::RenderQueue& queue) const;
    };

}

#endif
