

#ifndef __mugato__LabelFont__
#define __mugato__LabelFont__

#include <mugato/label/LabelCharacter.hpp>
#include <map>
#include <string>

namespace mugato {

    class LabelFont
    {
    public:
        typedef LabelCharacter Character;
        typedef std::map<std::string, Character> CharacterMap;
    private:
        CharacterMap _chars;
        float _lineHeight;
    public:
        LabelFont();

        void setCharacter(const std::string& name, const Character& chr);
        bool hasCharacter(const std::string& name) const;
        const Character& getCharacter(const std::string& name) const;
        Character& getCharacter(const std::string& name);

        std::shared_ptr<gorn::Material> getMaterial() const;
        void setMaterial(const std::shared_ptr<gorn::Material>& material);

        void setLineHeight(float val);
        float getLineHeight() const;
    };

}

#endif
