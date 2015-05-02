
#include <mugato/label/Label.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <glm/gtx/transform.hpp>

namespace mugato {

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
        _dirtySize = true;
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
            _dirtySize = true;
            _dirtyAlignment = true;
        }
    }

    const glm::vec2& Label::getSize() const
    {
        return _size;
    }

    glm::vec2 Label::getOriginalSize() const
    {
        glm::vec2 size;
        glm::vec2 line;
        for(auto& name : _characters)
        {
            auto& chr = _font->getCharacter(name);
            line += chr.getBase();
            line.x += chr.getRegion().getAdvance();
            if(chr.getMode() == LabelCharacter::Mode::Line)
            {
                size.y -= line.y;
                if(size.x < line.x)
                {
                    size.x = line.x;
                }
                line = glm::vec2();
            }
        }
        return size;
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
            _dirtySize = true;
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
        bool multiline = false;
        for(auto itr=_text.begin(); itr!=_text.end(); ++itr)
        {
            name += *itr;
            if(_font->hasCharacter(name))
            {
                if(name == "\n")
                {
                    multiline = true;
                }
                _characters.push_back(name);
                name.clear();
            }
        }
        if(!multiline)
        {
            _characters.push_back("\n");
        }
        _dirtyChars = false;
    }

    void Label::updateAlignmentTransform()
    {
        if(!_dirtyAlignment)
        {
            return;
        }
        auto osize = getOriginalSize();
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
        case Alignment::CenterLeft:
            t = glm::vec3(0.0f, (size.y+osize.y)*0.5f, 0.0f);
            break;
        case Alignment::Center:
            t = glm::vec3((size.x-osize.x)*0.5f, (size.y+osize.y)*0.5f, 0.0f);
            break;
        case Alignment::CenterRight:
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

    void Label::updateSize()
    {
        if(_dirtySize)
        {
            _dirtySize = false;
        }
    }

    void Label::update(double dt)
    {
        updateChars();
        updateAlignmentTransform();
        for(auto& name : _characters)
        {
            _font->getCharacter(name).update();
        }
    }
    
    void Label::render(gorn::RenderQueue& queue)
    {
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(_transform);
        // additional push checkpoint for linebreaks
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PushCheckpoint);
        for(auto& name : _characters)
        {
            auto& chr = _font->getCharacter(name);
            chr.render(queue);

        }
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PopCheckpoint);
        queue.addCommand().withTransformMode(
                gorn::RenderCommand::TransformMode::PopCheckpoint);

    }

}
