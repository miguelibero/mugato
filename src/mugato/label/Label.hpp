

#ifndef __mugato__Label__
#define __mugato__Label__

#include <mugato/label/LabelFont.hpp>
#include <mugato/label/LabelEnums.hpp>
#include <mugato/base/Alignment.hpp>
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
        static const char* kLineBreak;
        typedef LabelFont Font;
        typedef LabelResizeMode ResizeMode;
    private:

        std::shared_ptr<Font> _font;
        std::string _text;
        glm::vec2 _size;
        bool _dirtyChars;
        bool _dirtyAlignment;
        std::vector<std::string> _characters;
        Alignment _alignment;
        ResizeMode _resizeMode;
        glm::mat4 _transform;
        glm::vec2 _contentSize;
        std::vector<float> _lineWidths;

        void init();
        void updateChars();
        void updateAlignmentTransform();
        glm::vec3 getLineBreakTranslation(float contentWidth, float lineWidth);
    public:
        Label(const std::string& text="");
        Label(const std::shared_ptr<Font>& font);
        Label(const std::shared_ptr<Font>& font, const std::string& text);

        std::shared_ptr<gorn::Material> getMaterial() const;
        void setMaterial(const std::shared_ptr<gorn::Material>& material);

        const std::shared_ptr<Font>& getFont() const;
        void setFont(const std::shared_ptr<Font>& font);

        const std::string& getText() const;
        void setText(const std::string& text);

        const glm::vec2& getContentSize() const;
        std::vector<float> getLineWidths() const;

        const glm::vec2& getSize() const;
        void setSize(const glm::vec2& size);

        Alignment getAlignment() const;
        void setAlignment(Alignment value);

        ResizeMode getResizeMode() const;
        void setResizeMode(ResizeMode mode);
        
        void update(double dt);
        void render(gorn::RenderQueue& queue);
    };

}

#endif
