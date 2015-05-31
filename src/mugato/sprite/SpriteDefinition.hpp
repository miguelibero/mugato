

#ifndef __mugato__SpriteDefinition__
#define __mugato__SpriteDefinition__

#include <mugato/sprite/SpriteAnimationDefinition.hpp>
#include <mugato/sprite/SpriteEnums.hpp>
#include <string>
#include <map>
#include <glm/glm.hpp>

namespace mugato {

    class SpriteDefinition
    {
    public:
        typedef SpriteAnimationDefinition Animation;
        typedef std::map<std::string, Animation> AnimationMap;
        typedef SpriteResizeMode ResizeMode;
    private:
        std::string _atlas;
        std::string _material;
        glm::vec4 _stretchBorders;
        ResizeMode _resizeMode;
        AnimationMap _anims;
    public:
        SpriteDefinition();

        SpriteDefinition& withAtlas(const std::string& atlas);
        SpriteDefinition& withMaterial(const std::string& name);
        SpriteDefinition& withStretchBorders(const glm::vec4& borders);
        SpriteDefinition& withResizeMode(ResizeMode mode);
        SpriteDefinition& withAnimation(const std::string& name,
            const Animation& anim);
        Animation& setAnimation(const std::string& name);

        const std::string& getAtlas() const;
        const std::string& getMaterial() const;
        const Animation& getAnimation(const std::string& name) const;
        bool hasAnimation(const std::string& name) const;
        const AnimationMap& getAnimations() const;
        const glm::vec4& getStretchBorders() const;
        ResizeMode getResizeMode() const;
    };

}

#endif
