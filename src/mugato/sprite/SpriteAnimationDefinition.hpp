

#ifndef __mugato__SpriteAnimationDefinition__
#define __mugato__SpriteAnimationDefinition__

#include <string>
#include <vector>

namespace mugato {

    class SpriteAnimationDefinition
    {
    private:
        std::vector<std::string> _frames;
        std::string _atlas;
        double _frameDuration;
    public:
        SpriteAnimationDefinition();

        SpriteAnimationDefinition& withAtlas(const std::string& atlas);
        SpriteAnimationDefinition& withFrame(const std::string& frame);
        SpriteAnimationDefinition& withFrameDuration(double duration);

        const std::string& getAtlas() const;
        const std::vector<std::string>& getFrames() const;
        double getFrameDuration() const;
    };

}

#endif
