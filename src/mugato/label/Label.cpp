
#include <mugato/label/Label.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <glm/gtx/transform.hpp>
#include <algorithm>

namespace mugato {

    const char* Label::kLineBreak = "\n";
    const char* Label::kSpace = " ";

    Label::Label(const std::string& text):
    _text(text)
    {
        init();
    }

    Label::Label(const std::shared_ptr<Font>& font):
    _font(font) 
    {
        init();
    }

    Label::Label(const std::shared_ptr<Font>& font, const std::string& text):
    _font(font), _text(text)
    {
        init();
    }

    void Label::init()
    {
        _dirtyChars = true;
        _dirtyAlignment = true;
        _alignment = Alignment::Center;
        _resizeMode = ResizeMode::Original;
    }

    const std::shared_ptr<Label::Font>& Label::getFont() const
    {
        return _font;
    }

    void Label::setFont(const std::shared_ptr<Font>& font)
    {
        _font = font;
    }

    std::shared_ptr<gorn::Material> Label::getMaterial() const
    {
        if(_font)
        {
            return _font->getMaterial();
        }
        return nullptr;
    }

    void Label::setMaterial(const std::shared_ptr<gorn::Material>& material)
    {
        if(_font)
        {
            _font->setMaterial(material);
        }
    }


    void Label::setText(const std::string& text)
    {
        if(_text != text)
        {
            _text = text;
            _dirtyChars = true;
            _dirtyAlignment = true;
        }
    }

    const std::string& Label::getText() const
    {
        return _text;
    }

    void Label::setSize(const glm::vec2& size)
    {
        if(_size != size)
        {
            _size = size;
            _dirtyAlignment = true;
        }
    }

    const glm::vec2& Label::getSize() const
    {
        return _size;
    }

    const glm::vec2& Label::getContentSize() const
    {
        return _contentSize;
    }

    std::vector<float> Label::getLineWidths() const
    {
        return _lineWidths;
    }


    Alignment Label::getAlignment() const
    {
        return _alignment;
    }

    void Label::setAlignment(Alignment value)
    {
        if(_alignment != value)
        {
            _alignment = value;
            _dirtyAlignment = true;
        }
    }

    Label::ResizeMode Label::getResizeMode() const
    {
        return _resizeMode;
    }

    void Label::setResizeMode(ResizeMode mode)
    {
        if(_resizeMode != mode)
        {
            _resizeMode = mode;
            _dirtyAlignment = true;
        }
    }
    
    void Label::updateChars()
    {
        if(!_dirtyChars)
        {
            return;
        }
        _characters.clear();
        if(_font == nullptr)
        {
            return;
        }

        std::string chr;
        float w = 0.0f;
        float ww = 0.0f;
        std::vector<std::string> word;
        _lineWidths.clear();
        auto last = _text.end()-1;
        for(auto itr=_text.begin(); itr!=_text.end(); ++itr)
        {
            chr += *itr;
            bool wordend = false;
            bool lineend = false;
            bool chrend = false;
            if(chr == kLineBreak)
            {
                wordend = true;
                lineend = true;
                chrend = true;
            }
            else if(_font->hasCharacter(chr))
            {
                auto& fchr = _font->getCharacter(chr);
                ww += fchr.getRegion().getAdvance();
                word.push_back(chr);
                if(w + ww > _size.x)
                {
                    lineend = true;
                }
                chrend = true;
            }
            if(itr == last)
            {
                wordend = true;
                lineend = true;
                chrend = true;
            }
            if(chr == kSpace)
            {
                wordend = true;
                chrend = true;
            }
            if(wordend)
            {                
                _characters.insert(_characters.end(),
                  word.begin(), word.end());
                w += ww;
                word.clear();
                ww = 0.0f;
            }
            if(lineend)
            {
                _lineWidths.push_back(w);
                _characters.push_back(kLineBreak);
                w = 0.0f;
            }
            if(chrend)
            {
                chr.clear();
            }
        }
        _contentSize.x = *std::max_element(
            _lineWidths.begin(), _lineWidths.end());
        _contentSize.y = _font->getLineHeight()*_lineWidths.size();
        _dirtyChars = false;
    }

    void Label::updateAlignmentTransform()
    {
        if(!_dirtyAlignment)
        {
            return;
        }
        auto& c = getContentSize();
        _transform = glm::translate(glm::mat4(),
            alignPosition(_alignment, getSize(), c)
                +glm::vec3(0.0f, c.y, 0.0f));
        _dirtyAlignment = false;
    }

    void Label::update(double dt)
    {
        updateChars();
        updateAlignmentTransform();
        for(auto& name : _characters)
        {
            if(_font->hasCharacter(name))
            {
                _font->getCharacter(name).update();
            }
        }
    }

    glm::vec3 Label::getLineBreakTranslation(float cw, float lw)
    {
        if(_alignment == Alignment::Top ||
          _alignment == Alignment::Bottom ||
            _alignment == Alignment::Center)
        {
            return glm::vec3((cw-lw)*0.5f, 0.0f, 0.0f);
        }
        else if(_alignment == Alignment::TopRight ||
          _alignment == Alignment::Right ||
            _alignment == Alignment::BottomRight)
        {
            return glm::vec3((cw-lw), 0.0f, 0.0f);
        }
        else
        {
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }

    void Label::renderLineBreak(gorn::RenderQueue& queue, float w)
    {
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PopCheckpoint);
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(glm::translate(glm::mat4(),
                glm::vec3(0.0f, -_font->getLineHeight(), 0.0f)));
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(glm::translate(glm::mat4(),
                getLineBreakTranslation(_contentSize.x, w)));
    }
    
    void Label::render(gorn::RenderQueue& queue)
    {
        if(_font == nullptr)
        {
            return;
        }
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(_transform);
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PushCheckpoint);

        auto line = _lineWidths.begin();
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(glm::translate(glm::mat4(),
                getLineBreakTranslation(_contentSize.x, *line)));
        line++;
        for(auto& name : _characters)
        {
            if(name == kLineBreak)
            {
                if(line != _lineWidths.end())
                {
                    renderLineBreak(queue, *line);
                    line++;
                }
            }
            else if(_font->hasCharacter(name))
            {
                auto& chr = _font->getCharacter(name);
                chr.render(queue);
            }
        }

        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PopCheckpoint);
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PopCheckpoint);

    }

}
