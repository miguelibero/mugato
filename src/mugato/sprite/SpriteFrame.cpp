
#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <gorn/gl/Material.hpp>

namespace mugato {

    SpriteFrame::SpriteFrame()
    {
        init();
    }

    SpriteFrame::SpriteFrame(
        const std::shared_ptr<gorn::Material>& material):
    _material(material)
    {
        _region.setSize(material->getSize());
        init();
    }

    SpriteFrame::SpriteFrame(
        const std::shared_ptr<gorn::Material>& material, const Region& region):
    _material(material), _region(region)
    {
        init();
    }

    void SpriteFrame::init()
    {
        _resizeMode = ResizeMode::Original;
        _dirtySize = true;
        _dirtyTexVerts = true;
        _dirtyPosVerts = true;
    }

    void SpriteFrame::setMaterial(
        const std::shared_ptr<gorn::Material>& material)
    {
        if(_material != material)
        {
            _material = material;
            _dirtyTexVerts = true;
        }
    }

    const std::shared_ptr<gorn::Material>& SpriteFrame::getMaterial() const
    {
        return _material;
    }

    std::shared_ptr<gorn::Material>& SpriteFrame::getMaterial()
    {
        return _material;
    }

    void SpriteFrame::setRegion(const Region& region)
    {
        if(_region != region)
        {
            _region = region;
            _dirtyTexVerts = true;
            _dirtyPosVerts = true;
        }
    }

    const SpriteFrame::Region& SpriteFrame::getRegion() const
    {
        return _region;
    }

    const glm::vec2& SpriteFrame::getSize() const
    {
        updateSize();
        return _size;
    }

    void SpriteFrame::updateSize() const
    {
        if(_dirtySize)
        {
            auto& s = _region.getOriginalSize();
            switch(_resizeMode)
            {
            case ResizeMode::Original:
                _size = s;
                break;
            case ResizeMode::Inside:
                {
                    if(s.x < s.y)
                    {
                        _size.x = s.x*_size.y/s.y;
                    }
                    else
                    {
                        _size.y = s.y*_size.x/s.x;
                    }
                }
                break;
            case ResizeMode::Outside:
                {
                    auto& s = _region.getOriginalSize();
                    if(s.x > s.y)
                    {
                        _size.x = s.x*_size.y/s.y;
                    }
                    else
                    {
                        _size.y = s.y*_size.x/s.x;
                    }
                }
                break;
            default:
                break;
            }
            _dirtySize = false;
        }
    }

    void SpriteFrame::setStretchBorders(const glm::vec4& borders)
    {
        if(_stretchBorders != borders)
        {
            _stretchBorders = borders;
            _dirtyPosVerts = true;
            _dirtyTexVerts = true;
        }
    }

    bool SpriteFrame::hasStretchBorders() const
    {
        auto& b = _stretchBorders;
        return b[0] != 0.0f || b[1] != 0.0f || b[2] != 0.0f || b[3] != 0.0f;
    }

    void SpriteFrame::setSize(const glm::vec2& size)
    {
        if(_size != size)
        {
            _size = size;
            _dirtySize = true;
        }
    }

    void SpriteFrame::setResizeMode(ResizeMode mode)
    {
        _resizeMode = mode;
    }

    void SpriteFrame::update()
    {
        updateSize();
        updatePositionData();
        updateTextureData();
    }

    void SpriteFrame::updatePositionData()
    {
        if(!_dirtyPosVerts)
        {
            return;
        }
        auto size = _size;
        glm::vec2 rsize = size;
        if(!_region.empty())
        {
            rsize = rsize/_region.getOriginalSize()*_region.getSize();
        }
        auto bl = (size-rsize)*0.5f+_region.getOffset();
        auto tr = bl + rsize;

        _posVerts = {
            bl.x, bl.y,
            tr.x, bl.y,
            bl.x, tr.y,
            tr.x, tr.y
        };

        if(hasStretchBorders())
        {
            auto& b = _stretchBorders;
            _posVerts.append({
                bl.x + b[1], bl.y + b[2],
                tr.x - b[3], bl.y + b[2],
                bl.x + b[1], tr.y - b[0],
                tr.x - b[3], tr.y - b[0],
                bl.x + b[1], bl.y       ,
                bl.x       , bl.y + b[2],
                tr.x - b[3], bl.y       ,
                tr.x       , bl.y + b[2],
                bl.x       , tr.y - b[0],
                bl.x + b[1], tr.y       ,
                tr.x       , tr.y - b[0],
                tr.x - b[3], tr.y
            });
        }
        _dirtyPosVerts = false;
    }

    void SpriteFrame::updateTextureData()
    {
        if(!_dirtyTexVerts || _material == nullptr)
        {
            return;
        }

        auto bl = _region.getPosition();
        auto origin = _region.getOrigin();
        auto rsize = _region.getSize();
        auto msize = _material->getSize();
        auto rotate = _region.getRotate();
        if(rotate)
        {
            std::swap(rsize.x, rsize.y);
        }
        if(origin == Region::Origin::TopLeft || origin == Region::Origin::TopRight)
        {
            bl.y = msize.y - rsize.y - bl.y;
        }
        if(origin == Region::Origin::BottomRight || origin == Region::Origin::TopRight)
        {
            bl.x = msize.x - rsize.x - bl.x;
        }
        auto tr = bl+rsize;
        tr /= msize;
        bl /= msize;

        if(rotate)
        {
            _texVerts = {
                tr.x, bl.y,
                tr.x, tr.y,
                bl.x, bl.y,
                bl.x, tr.y
            };
        }
        else
        {
            _texVerts = {
                bl.x, bl.y,
                tr.x, bl.y,
                bl.x, tr.y,
                tr.x, tr.y
            };
        }

        if(hasStretchBorders())
        {
            auto b = _stretchBorders;
            b[0] /= msize.y;
            b[1] /= msize.x;
            b[2] /= msize.y;
            b[3] /= msize.x;
            _texVerts.append({
                bl.x + b[1], bl.y + b[2],
                tr.x - b[3], bl.y + b[2],
                bl.x + b[1], tr.y - b[0],
                tr.x - b[3], tr.y - b[0],
                bl.x + b[1], bl.y,
                bl.x       , bl.y + b[2],
                tr.x - b[3], bl.y       ,
                tr.x       , bl.y + b[2],
                bl.x       , tr.y - b[0],
                bl.x + b[1], tr.y       ,
                tr.x       , tr.y - b[0],
                tr.x - b[3], tr.y
            });
        }

        _dirtyTexVerts = false;
    }

    void SpriteFrame::render(gorn::RenderQueue& queue) const
    {
        gorn::RenderCommand::Elements elms;
        if(!hasStretchBorders())
        {
            elms = {
                0, 1, 2, 2, 3, 1
            };
        }
        else
        {
            elms = {
                 4,  5,  6,  6,  7,  5,
                 0,  8,  9,  9,  4,  8,
                 8, 10,  4,  4,  5, 10,
                10,  1,  5,  5, 11,  1,
                 5, 11,  7,  7, 14, 11,
                 7, 14, 15, 15,  3, 14,
                 6,  7, 13, 13, 15,  7,
                12,  6,  2,  2, 13,  6,
                 9,  4, 12, 12,  6,  4
            };
        }

        queue.addCommand()
            .withMaterial(_material)
            .withAttribute(gorn::AttributeKind::Position, 
                _posVerts, 2, gorn::BasicType::Float)
            .withAttribute(gorn::AttributeKind::TexCoords, 
                _texVerts, 2, gorn::BasicType::Float)
            .withElements(std::move(elms));
    }
}
