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
        size_t sizeNodes() const;

        template<typename Filter>
        bool clear(Filter f);

        template<typename Filter>
        void find(Elements& elms, Filter f) const;

        template<typename Filter>
        bool empty(Filter f) const;
        
        template<typename Filter>
        size_t size(Filter f) const;

        gorn::Data getElementsVertices(DrawMode mode=DrawMode::Triangles) const;
        gorn::Data getNodesVertices(DrawMode mode=DrawMode::Triangles) const;
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
        auto itr = std::remove(_elements.begin(), _elements.end(), elm);
        if(itr != _elements.end())
        {
            _elements.erase(itr, _elements.end());
            join();
            return true;
        }
        for(auto itr = _branches.begin(); itr != _branches.end(); ++itr)
        {
            if((*itr)->remove(elm))
            {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool QuadTreeNode<T>::clear()
    {
        return clear([](const Element&){
            return true;
        });
    }

    template<typename T>
    void QuadTreeNode<T>::find(Elements& elms) const
    {
        return find(elms, [](const Element&){
            return true;
        });
    }

    template<typename T>
    bool QuadTreeNode<T>::empty() const
    {
        return empty([](const Element&){
            return true;
        });
    }

    template<typename T>
    size_t QuadTreeNode<T>::size() const
    {
        return size([](const Element&){
            return true;
        });
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
    template<typename Filter>
    bool QuadTreeNode<T>::clear(Filter filter)
    {
        bool change = false;
        if(filter)
        {
            auto itr = std::remove_if(_elements.begin(), _elements.end(), filter);
            if(itr != _elements.end())
            {
                _elements.erase(itr, _elements.end());
                change = true;
            }
        }
        else
        {
            change = !_elements.empty();
            _elements.clear();
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
            if (!filter || filter(elm))
            {
                insert(elm, elms);
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
            if (!filter || filter(elm))
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
        size_t size = 0;
        if(filter)
        {
            size += std::count_if(
                _elements.begin(), _elements.end(), filter);
        }
        else
        {
            size += _elements.size();
        }
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
}

#endif

