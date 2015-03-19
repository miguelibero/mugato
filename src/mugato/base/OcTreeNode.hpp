#ifndef __mugato__OcTreeNode__
#define __mugato__OcTreeNode__

#include <memory>
#include <algorithm>
#include <mugato/base/Rectangle.hpp>
#include <mugato/base/OcTreeElement.hpp>
#include <buffer.hpp>
#include <buffer_writer.hpp>

namespace mugato
{
    template<typename T>
    class OcTreeNode
    {
    public:
        typedef OcTreeElement<T> Element;
        typedef std::vector<Element> Elements;
        typedef OcTreeNode<T> Node;
        typedef std::vector<std::unique_ptr<Node>> Branches;
        typedef Rectangle::DrawMode DrawMode;
        typedef Rectangle::MatchType MatchType;
    private:
        Rectangle _area;
        Elements _elements;
        Node* _parent;
        Branches _branches;
        size_t _maxElements;
        glm::vec3 _divisions;
        MatchType _matchType;

        bool split();
        bool join();

    public:
        OcTreeNode(const Rectangle& area, Node* parent,
            size_t max, const glm::vec3& divs);

        const Rectangle& getArea() const;
        void setArea(const Rectangle& area);

        bool insert(const Element& elm);
        bool remove(const Element& elm);
        bool adjust(bool all=true);

        bool clear();
        void find(Elements& elms) const;
        bool empty() const;
        size_t size() const;

        bool clear(const RectangleMatch& match);
        void find(Elements& elms, const RectangleMatch& match) const;
        bool empty(const RectangleMatch& match) const;
        size_t size(const RectangleMatch& match) const;

        template<typename Filter>
        bool clear(Filter f);

        template<typename Filter>
        void find(Elements& elms, Filter f) const;

        template<typename Filter>
        bool empty(Filter f) const;
        
        template<typename Filter>
        size_t size(Filter f) const;

        size_t sizeNodes() const;

        buffer getElementsVertices(DrawMode mode=DrawMode::Triangles) const;
        buffer getNodesVertices(DrawMode mode=DrawMode::Triangles) const;
        buffer getElementsVertices(const RectangleMatch& match,
            DrawMode mode=DrawMode::Triangles) const;
        buffer getNodesVertices(const RectangleMatch& match,
            DrawMode mode=DrawMode::Triangles) const;
    };

    template<typename T>
    OcTreeNode<T>::OcTreeNode(const Rectangle& area, Node* parent,
            size_t max, const glm::vec3& divs):
        _area(area), _parent(parent),
        _maxElements(max), _divisions(divs),
        _matchType(MatchType::Overlap)
    {
    }

    template<typename T>
    const Rectangle& OcTreeNode<T>::getArea() const
    {
        return _area;
    }

    template<typename T>
    void OcTreeNode<T>::setArea(const Rectangle& area)
    {
        _area = area;
    }

    template<typename T>
    bool OcTreeNode<T>::insert(const Element& elm)
    {
        remove(elm);
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->insert(elm))
            {
                return true;
            }
        }
        if(_area.matches(elm.getArea(), _matchType))
        {
            _elements.push_back(elm);
            return true;
        }
        return false;
    }

    template<typename T>
    bool OcTreeNode<T>::remove(const Element& elm)
    {
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->remove(elm))
            {
                return true;
            }
        }
        auto itr = std::remove(_elements.begin(), _elements.end(), elm);
        if(itr != _elements.end())
        {
            _elements.erase(itr, _elements.end());
            return true;
        }
        return false;
    }

    template<typename T>
    bool OcTreeNode<T>::clear()
    {
        bool changed = !_elements.empty();
        _elements.clear();
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->clear())
            {
                changed = true;
            }
        }
        _branches.clear();
        return changed;
    }

    template<typename T>
    void OcTreeNode<T>::find(Elements& elms) const
    {
        elms.insert(elms.end(), _elements.begin(), _elements.end());
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            (*itr)->find(elms);
        }
    }

    template<typename T>
    bool OcTreeNode<T>::empty() const
    {
        if(!_elements.empty())
        {
            return false;
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if(!(*itr)->empty())
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    size_t OcTreeNode<T>::size() const
    {
        size_t size = _elements.size();
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->size();
        }
        return size;
    }

    template<typename T>
    size_t OcTreeNode<T>::sizeNodes() const
    {
        size_t size = 1;
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->sizeNodes();
        }
        return size;
    }

    template<typename T>
    bool OcTreeNode<T>::clear(const RectangleMatch& match)
    {
        bool changed = false;
        if(!match.matches(_area))
        {
            return changed;
        }
        auto itr = std::remove_if(_elements.begin(), _elements.end(),
            [&match](const Element& elm){
            return match.matches(elm.getArea());
        });
        if(itr != _elements.end())
        {
            _elements.erase(itr, _elements.end());
            changed = true;
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->clear(match))
            {
                changed = true;
            }
        }
        return changed;
    }

    template<typename T>
    void OcTreeNode<T>::find(Elements& elms, const RectangleMatch& match) const
    {
        if(!match.matches(_area))
        {
            return;
        }
        for (auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            const auto& elm = *itr;
            if (match.matches(elm.getArea()))
            {
                elms.push_back(elm);
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            (*itr)->find(elms, match);
        }
    }

    template<typename T>
    bool OcTreeNode<T>::empty(const RectangleMatch& match) const
    {
        if(!match.matches(_area))
        {
            return true;
        }
        for (auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            const auto& elm = *itr;
            if (match.matches(elm.getArea()))
            {
                return false;
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if(!(*itr)->empty(match))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    size_t OcTreeNode<T>::size(const RectangleMatch& match) const
    {
        if(!match.matches(_area))
        {
            return 0;
        }
        size_t size = std::count_if(
                _elements.begin(), _elements.end(),
            [&match](const Element& elm){
                return match.matches(elm.getArea());
        });
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->size(match);
        }
        return size;
    }

    template<typename T>
    template<typename Filter>
    bool OcTreeNode<T>::clear(Filter filter)
    {
        bool changed = false;
        auto itr = std::remove_if(_elements.begin(), _elements.end(), filter);
        if(itr != _elements.end())
        {
            _elements.erase(itr, _elements.end());
            changed = true;
        }

        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->clear(filter))
            {
                changed = true;
            }
        }
        return changed;
    }

    template<typename T>
    template<typename Filter>
    void OcTreeNode<T>::find(Elements& elms, Filter filter) const
    {
        for (auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            const auto& elm = *itr;
            if (filter(elm))
            {
                elms.push_back(elm);
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            (*itr)->find(elms, filter);
        }
    }

    template<typename T>
    template<typename Filter>
    bool OcTreeNode<T>::empty(Filter filter) const
    {
        for (auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            const auto& elm = *itr;
            if (filter(elm))
            {
                return false;
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if(!(*itr)->empty(filter))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    template<typename Filter>
    size_t OcTreeNode<T>::size(Filter filter) const
    {
        size_t size = std::count_if(
                _elements.begin(), _elements.end(), filter);
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->size(filter);
        }
        return size;
    }

    template<typename T>
    bool OcTreeNode<T>::split()
    {
        if(_maxElements == 0)
        {
            return false;
        }
        if(!_branches.empty())
        {
            bool changed = false;
            for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
            {
                if((*itr)->split())
                {
                    changed =  true;
                }
            }
            return changed;
        }
        std::vector<glm::vec3> origins;
        for(auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            auto& p = itr->getArea().origin;
            auto fitr = std::find(origins.begin(), origins.end(), p);
            if(fitr == origins.end())
            {
                origins.insert(fitr, p);
            }
        }
        if(origins.size() <= _maxElements)
        {
            return false;
        }
        auto parts = _area.divide(_divisions);
        _branches.reserve(parts.size());
        for(auto itr = parts.begin(); itr != parts.end(); ++itr)
        {
            _branches.push_back(std::unique_ptr<Node>(
                new Node(*itr, this, _maxElements, _divisions)));
        }
        _elements.erase(std::remove_if(_elements.begin(), _elements.end(),
            [this](Element& elm)
            {
                for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
                {
                    if((*itr)->insert(elm))
                    {
                        return true;
                    }
                }
                return false;
            }), _elements.end());
        return true;
    }

    template<typename T>
    bool OcTreeNode<T>::join()
    {
        if(_maxElements == 0 || _branches.empty())
        {
            return false;
        }
        size_t size = _elements.size();
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->join())
            {
                return true;
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->sizeNodes() > 1)
            {
                return false;
            }
            size += (*itr)->_elements.size();
        }
        if(size > _maxElements)
        {
            return false;
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            auto& elms = (*itr)->_elements;
            _elements.insert(_elements.end(), elms.begin(), elms.end());
        }
        _branches.clear();
        return true;
    }

    template<typename T>
    bool OcTreeNode<T>::adjust(bool all)
    {
        bool changed = false;
        if(all)
        {
            while(adjust(false))
            {
                changed = true;
            }
        }
        else
        {
            if(join())
            {
                changed = true;
            }
            if(split())
            {
                changed = true;
            }
        }
        return changed;
    }

    template<typename T>
    buffer OcTreeNode<T>::getElementsVertices(DrawMode mode) const
    {
        buffer data;
        buffer_writer out(data);
        
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            out.write((*itr)->getElementsVertices(mode));
        }
        for(auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            out.write(itr->getArea().getVertices(mode));
        }
        return data;
    }

    template<typename T>
    buffer OcTreeNode<T>::getNodesVertices(DrawMode mode) const
    {
        buffer data;
        buffer_writer out(data);
        
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            out.write((*itr)->getNodesVertices(mode));
        }
        out.write(_area.getVertices(mode));

        return data;
    }

    template<typename T>
    buffer OcTreeNode<T>::getElementsVertices(const RectangleMatch& match,
        DrawMode mode) const
    {
        buffer data;
        buffer_writer out(data);
        
        if(match.matches(_area))
        {
            for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
            {
                out.write((*itr)->getElementsVertices(match, mode));
            }
            for(auto itr = _elements.begin(); itr != _elements.end(); ++itr)
            {
                const auto& earea = itr->getArea();
                if(match.matches(earea))
                {
                    out.write(earea.getVertices(mode));
                }
            }
        }
        return data;
    }

    template<typename T>
    buffer OcTreeNode<T>::getNodesVertices(const RectangleMatch& match,
        DrawMode mode) const
    {
        buffer data;
        if(match.matches(_area))
        {
            buffer_writer out(data);
            for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
            {
                out.write((*itr)->getNodesVertices(match, mode));
            }
            out.write(_area.getVertices(mode));
        }

        return data;
    }
}

#endif

