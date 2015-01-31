

#ifndef __mugato__LabelFont__
#define __mugato__LabelFont__

#include <mugato/label/LabelCharacter.hpp>
#include <map>

namespace mugato {

    class LabelFont
    {
    public:
        typedef LabelCharacter Character;
        typedef std::map<std::string, Character> CharacterMap;
    private:
        CharacterMap _chars;      
    public:
        LabelFont();

        void setCharacter(const std::string& name, const Character& chr);
        bool hasCharacter(const std::string& name) const;
        const Character& getCharacter(const std::string& name) const;
        Character& getCharacter(const std::string& name);
    };

}

#endif
