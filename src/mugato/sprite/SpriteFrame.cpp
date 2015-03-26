
#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/Material.hpp>
#include <gorn/render/Kinds.hpp>

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

    const glm::vec2 SpriteFrame::getSize() const
    {
        return _region.getOriginalSize();
    }

    void SpriteFrame::update()
    {
        updatePositionData();
        updateTextureData();
    }

    void SpriteFrame::updatePositionData()
    {
        if(!_dirtyPosVerts)
        {
            return;
        }
        auto osize = _region.getOriginalSize();
        auto rsize = _region.getSize();
        auto bl = (osize-rsize)*0.5f+_region.getOffset();
        auto tr = bl + rsize;

        _posVerts = {
            bl.x, tr.y,
            tr.x, tr.y,
            tr.x, bl.y,
            bl.x, bl.y
        };
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
                bl.x, bl.y,
                bl.x, tr.y,
                tr.x, tr.y,
                tr.x, bl.y
            };
        }
        else
        {
            _texVerts = {
                bl.x, tr.y,
                tr.x, tr.y,
                tr.x, bl.y,
                bl.x, bl.y
            };
        }
        _dirtyTexVerts = false;
    }

    void SpriteFrame::render(gorn::RenderQueue& queue) const
    {
        queue.addCommand()
            .withMaterial(_material)
            .withAttribute(gorn::AttributeKind::Position, 
                _posVerts, 2, gorn::BasicType::Float)
            .withAttribute(gorn::AttributeKind::TexCoords, 
                _texVerts, 2, gorn::BasicType::Float)
            .withElements({
                0, 1, 2,
                2, 3, 0
            });
    }
}
