

#ifndef __mugato__SpriteDefinition__
#define __mugato__SpriteDefinition__

#include <mugato/sprite/SpriteAnimationDefinition.hpp>
#include <string>
#include <map>

namespace mugato {

    class SpriteDefinition
    {
    public:
        typedef SpriteAnimationDefinition Animation;
        typedef std::map<std::string, Animation> AnimationMap;
    private:
        std::string _atlas;
        std::string _material;
        AnimationMap _anims;
    public:
        SpriteDefinition();

        SpriteDefinition& withAtlas(const std::string& atlas);
        SpriteDefinition& withMaterial(const std::string& name);
        SpriteDefinition& withAnimation(const std::string& name,
            const Animation& anim);
        Animation& setAnimation(const std::string& name);

        const std::string& getAtlas() const;
        const std::string& getMaterial() const;
        const Animation& getAnimation(const std::string& name) const;
        bool hasAnimation(const std::string& name) const;
        const AnimationMap& getAnimations() const;
    };

}

#endif
