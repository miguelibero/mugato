
#include <mugato/label/Label.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <glm/gtx/transform.hpp>
#include <algorithm>

namespace mugato {

    const char* Label::kLineBreak = "\n";

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
        _alignment = Alignment::BottomLeft;
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

    glm::vec2 Label::getContentSize() const
    {
        return _contentSize;
    }

    std::vector<float> Label::getLineWidths() const
    {
        return _lineWidths;
    }


    Label::Alignment Label::getAlignment() const
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

        std::string name;
        for(auto itr=_text.begin(); itr!=_text.end(); ++itr)
        {
            name += *itr;
            if(name == kLineBreak || _font->hasCharacter(name))
            {
                _characters.push_back(name);
                name.clear();
            }
        }
        float w = 0.0f;
        _lineWidths.clear();
        for(auto& name : _characters)
        {
            if(name == kLineBreak)
            {
                _lineWidths.push_back(w);
                w = 0.0f;
            }
            else if(_font->hasCharacter(name))
            {
                auto& chr = _font->getCharacter(name);
                w += chr.getRegion().getAdvance();
            }
        }
        _lineWidths.push_back(w);
        _contentSize = glm::vec2(
            *std::max_element(_lineWidths.begin(), _lineWidths.end()),
            _font->getLineHeight()*_lineWidths.size());
        _dirtyChars = false;
    }

    void Label::updateAlignmentTransform()
    {
        if(!_dirtyAlignment)
        {
            return;
        }
        auto osize = getContentSize();
        auto& size = getSize();
        glm::vec3 t;
        switch(_alignment)
        {
        case Alignment::TopLeft:
            t = glm::vec3(0.0f, size.y, 0.0f);
            break;
        case Alignment::TopCenter:
            t = glm::vec3((size.x-osize.x)*0.5f, size.y, 0.0f);
            break;
        case Alignment::TopRight:
            t = glm::vec3(size.x-osize.x, size.y, 0.0f);
            break;
        case Alignment::Left:
            t = glm::vec3(0.0f, (size.y+osize.y)*0.5f, 0.0f);
            break;
        case Alignment::Center:
            t = glm::vec3((size.x-osize.x)*0.5f, (size.y+osize.y)*0.5f, 0.0f);
            break;
        case Alignment::Right:
            t = glm::vec3(size.x-osize.x, (size.y+osize.y)*0.5f, 0.0f);
            break;
        case Alignment::BottomLeft:
            t = glm::vec3(0.0f, osize.y, 0.0f);
            break;
        case Alignment::BottomCenter:
            t = glm::vec3((size.x-osize.x)*0.5f, osize.y, 0.0f);
            break;
        case Alignment::BottomRight:
            t = glm::vec3((size.x-osize.x), osize.y, 0.0f);
            break;
        default:
            break;
        }
        _transform = glm::translate(glm::mat4(), t);
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
        if(_alignment == Alignment::TopCenter ||
          _alignment == Alignment::BottomCenter ||
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
    
    void Label::render(gorn::RenderQueue& queue)
    {
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
            if(name == kLineBreak && line != _lineWidths.end())
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
                        getLineBreakTranslation(_contentSize.x, *line)));
                line++;
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
