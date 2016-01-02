

#include <mugato/label/LabelFont.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato {

    LabelFont::LabelFont():
    _lineHeight(0.0f)
    {
    }

    void LabelFont::setCharacter(
        const std::string& name, const Character& chr)
    {
        _chars[name] = chr;
    }

    bool LabelFont::hasCharacter(const std::string& name) const
    {
        return _chars.find(name) != _chars.end();
    }

    const LabelFont::Character& LabelFont::getCharacter(
        const std::string& name) const
    {
        return _chars.at(name);
    }

    LabelFont::Character& LabelFont::getCharacter(
        const std::string& name)
    {
        return _chars.at(name);
    }

    std::shared_ptr<gorn::Material> LabelFont::getMaterial() const
    {
        std::shared_ptr<gorn::Material> m;
        for(auto& pair : _chars)
        {
            if(m == nullptr)
            {
                m = pair.second.getMaterial();
            }
            else if(m != pair.second.getMaterial())
            {
                throw new Exception("LabelCharacters have different materials.");
            }
        }
        return m;
    }

    void LabelFont::setMaterial(const std::shared_ptr<gorn::Material>& material)
    {
        for(auto& pair : _chars)
        {
            pair.second.setMaterial(material);
        }
    }

    void LabelFont::setLineHeight(float val)
    {
        _lineHeight = val;
    }

    float LabelFont::getLineHeight() const
    {
        return _lineHeight;
    }
}

