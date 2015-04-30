

#ifndef __mugato__Label__
#define __mugato__Label__

#include <mugato/label/LabelFont.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace gorn {
    class RenderQueue;
}

namespace mugato {

    class Label
    {
    public:
        typedef LabelFont Font;
    private:

        std::shared_ptr<Font> _font;
        std::string _text;
        glm::vec2 _size;
        bool _dirty;
        std::vector<std::string> _characters;

        void init();

    public:
        Label(const std::string& text="");
        Label(const std::shared_ptr<Font>& font);
        Label(const std::shared_ptr<Font>& font, const std::string& text);

        const std::shared_ptr<Font>& getFont() const;
        void setFont(const std::shared_ptr<Font>& font);

        const std::string& getText() const;
        void setText(const std::string& text);

        const glm::vec2& getSize();
        void setSize(const glm::vec2& size);
        
        void update(double dt);
        void render(gorn::RenderQueue& queue);
    };

}

#endif
