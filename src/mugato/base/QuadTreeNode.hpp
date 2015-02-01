#ifndef __mugato__QuadTreeNode__
#define __mugato__QuadTreeNode__

#include <memory>
#include <algorithm>
#include <mugato/base/Rectangle.hpp>
#include <mugato/base/QuadTreeElement.hpp>
#include <gorn/base/Data.hpp>

namespace mugato
{
    template<typename T>
    class QuadTreeNode
    {
    public:
        typedef QuadTreeElement<T> Element;
        typedef std::vector<Element> Elements;
        typedef QuadTreeNode<T> Node;
        typedef Rectangle Area;
        typedef std::vector<std::unique_ptr<Node>> Branches;
        typedef Rectangle::DrawMode DrawMode;
    private:
        Area _area;
        Elements _elements;
        Node* _parent;
        Branches _branches;
        size_t _maxElements;
        glm::vec2 _divisions;

        bool split();
        bool join();

    public:
        QuadTreeNode(const Area& area, Node* parent,
            size_t max, const glm::vec2& divs);

        const Area& getArea() const;

        bool insert(const Element& elm);
        bool remove(const Element& elm);

        bool clear();
        void find(Elements& elms) const;
        bool empty() const;
        size_t size() const;

        bool clear(const Area& area, bool contained=false);
        void find(Elements& elms, const Area& area, bool contained=false) const;
        bool empty(const Area& area, bool contained=false) const;
        size_t size(const Area& area, bool contained=false) const;

        template<typename Filter>
        bool clear(Filter f);

        template<typename Filter>
        void find(Elements& elms, Filter f) const;

        template<typename Filter>
        bool empty(Filter f) const;
        
        template<typename Filter>
        size_t size(Filter f) const;

        size_t sizeNodes() const;

        gorn::Data getElementsVertices(DrawMode mode=DrawMode::Triangles) const;
        gorn::Data getNodesVertices(DrawMode mode=DrawMode::Triangles) const;
        gorn::Data getElementsVertices(const Area& area, bool contained=false,
            DrawMode mode=DrawMode::Triangles) const;
        gorn::Data getNodesVertices(const Area& area, bool contained=false,
            DrawMode mode=DrawMode::Triangles) const;
    };

    template<typename T>
    QuadTreeNode<T>::QuadTreeNode(const Area& area, Node* parent,
            size_t max, const glm::vec2& divs):
        _area(area), _parent(parent),
        _maxElements(max), _divisions(divs)
    {
    }

    template<typename T>
    const typename QuadTreeNode<T>::Area& QuadTreeNode<T>::getArea() const
    {
        return _area;
    }

    template<typename T>
    bool QuadTreeNode<T>::insert(const Element& elm)
    {
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->insert(elm))
            {
                return true;
            }
        }
        if(_area.intersects(elm.getArea()))
        {
            _elements.push_back(elm);
            split();
            return true;
        }
        return false;
    }

    template<typename T>
    bool QuadTreeNode<T>::remove(const Element& elm)
    {
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->remove(elm))
            {
                return true;
            }
        }
        if(_area.intersects(elm.getArea()))
        {
            auto itr = std::remove(_elements.begin(), _elements.end(), elm);
            if(itr != _elements.end())
            {
                _elements.erase(itr, _elements.end());
                join();
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool QuadTreeNode<T>::clear()
    {
        bool change = !_elements.empty();
        _elements.clear();
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->clear())
            {
                change = true;
            }
        }
        _branches.clear();
        return change;
    }

    template<typename T>
    void QuadTreeNode<T>::find(Elements& elms) const
    {
        elms.insert(elms.end(), _elements.begin(), _elements.end());
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            (*itr)->find(elms);
        }
    }

    template<typename T>
    bool QuadTreeNode<T>::empty() const
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
    size_t QuadTreeNode<T>::size() const
    {
        size_t size = _elements.size();
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->size();
        }
        return size;
    }

    template<typename T>
    size_t QuadTreeNode<T>::sizeNodes() const
    {
        size_t size = 1;
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->sizeNodes();
        }
        return size;
    }

    template<typename T>
    bool QuadTreeNode<T>::clear(const Area& area, bool contained)
    {
        bool change = false;
        if(!area.intersects(_area))
        {
            return change;
        }
        auto itr = std::remove_if(_elements.begin(), _elements.end(),
            [&area, contained](const Element& elm){
            return area.matches(elm.getArea(), contained);
        });
        if(itr != _elements.end())
        {
            _elements.erase(itr, _elements.end());
            change = true;
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->clear(area, contained))
            {
                change = true;
            }
        }
        if(change)
        {
            join();
        }
        return change;
    }

    template<typename T>
    void QuadTreeNode<T>::find(Elements& elms,
        const Area& area, bool contained) const
    {
        if(!area.intersects(_area))
        {
            return;
        }
        for (auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            const auto& elm = *itr;
            if (area.matches(elm.getArea(), contained))
            {
                elms.push_back(elm);
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            (*itr)->find(elms, area, contained);
        }
    }

    template<typename T>
    bool QuadTreeNode<T>::empty(const Area& area, bool contained) const
    {
        if(!area.intersects(_area))
        {
            return true;
        }
        for (auto itr = _elements.begin(); itr != _elements.end(); ++itr)
        {
            const auto& elm = *itr;
            if (area.matches(elm.getArea(), contained))
            {
                return false;
            }
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if(!(*itr)->empty(area, contained))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    size_t QuadTreeNode<T>::size(const Area& area, bool contained) const
    {
        if(!area.intersects(_area))
        {
            return 0;
        }
        size_t size = std::count_if(
                _elements.begin(), _elements.end(),
            [&area, contained](const Element& elm){
                return area.matches(elm.getArea(), contained);
        });
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->size(area, contained);
        }
        return size;
    }

    template<typename T>
    template<typename Filter>
    bool QuadTreeNode<T>::clear(Filter filter)
    {
        bool change = false;
        auto itr = std::remove_if(_elements.begin(), _elements.end(), filter);
        if(itr != _elements.end())
        {
            _elements.erase(itr, _elements.end());
            change = true;
        }

        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->clear(filter))
            {
                change = true;
            }
        }
        if(change)
        {
            join();
        }
        return change;
    }

    template<typename T>
    template<typename Filter>
    void QuadTreeNode<T>::find(Elements& elms, Filter filter) const
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
    bool QuadTreeNode<T>::empty(Filter filter) const
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
    size_t QuadTreeNode<T>::size(Filter filter) const
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
    bool QuadTreeNode<T>::split()
    {
        if(_maxElements == 0 || _elements.size() < _maxElements || !_branches.empty())
        {
            return false;
        }
        _branches.clear();
        _branches.reserve(_divisions.x*_divisions.y);

        auto min = _area.min();
        auto max = _area.max();
        auto size = _area.size/_divisions;
        auto p = min;
        for(p.y=min.y;p.y<max.y;p.y+=size.y)
        {
            for(p.x=min.x;p.x<max.x;p.x+=size.x)
            {
                _branches.push_back(std::unique_ptr<Node>(
                    new Node(Area(p, size), this, _maxElements, _divisions)));
            }
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
    bool QuadTreeNode<T>::join()
    {
        if(_maxElements == 0)
        {
            return false;
        }
        size_t size = _elements.size();
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            size += (*itr)->_elements.size();
        }
        if(size >= _maxElements)
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
    gorn::Data QuadTreeNode<T>::getElementsVertices(DrawMode mode) const
    {
        gorn::Data data;
        gorn::DataOutputStream out(data);
        
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
    gorn::Data QuadTreeNode<T>::getNodesVertices(DrawMode mode) const
    {
        gorn::Data data;
        gorn::DataOutputStream out(data);
        
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            out.write((*itr)->getNodesVertices(mode));
        }
        out.write(_area.getVertices(mode));

        return data;
    }

    template<typename T>
    gorn::Data QuadTreeNode<T>::getElementsVertices(const Area& area,
        bool contained, DrawMode mode) const
    {
        gorn::Data data;
        gorn::DataOutputStream out(data);
        
        if(area.intersects(_area))
        {
            for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
            {
                out.write((*itr)->getElementsVertices(area, contained, mode));
            }
            for(auto itr = _elements.begin(); itr != _elements.end(); ++itr)
            {
                const auto& earea = itr->getArea();
                if(area.matches(earea, contained))
                {
                    out.write(earea.getVertices(mode));
                }
            }
        }
        return data;
    }

    template<typename T>
    gorn::Data QuadTreeNode<T>::getNodesVertices(const Area& area,
        bool contained, DrawMode mode) const
    {
        gorn::Data data;
        if(area.matches(_area, contained))
        {
            gorn::DataOutputStream out(data);
            for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
            {
                out.write((*itr)->getNodesVertices(area, contained, mode));
            }
            out.write(_area.getVertices(mode));
        }

        return data;
    }
}

#endif

