#ifndef __mugato__OcTreeNode__
#define __mugato__OcTreeNode__

#include <memory>
#include <algorithm>
#include <vector>
#include <gorn/base/Rect.hpp>
#include <mugato/base/OcTreeElement.hpp>

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
        typedef gorn::Rect::MatchType MatchType;
    private:
        gorn::Rect _area;
        Elements _elements;
        Node* _parent;
        Branches _branches;
        size_t _maxElements;
        glm::vec3 _divisions;
        MatchType _matchType;

        bool split();
        bool join();

    public:
        OcTreeNode(const gorn::Rect& area, Node* parent,
            size_t max, const glm::vec3& divs);

        const gorn::Rect& getArea() const;
        void setArea(const gorn::Rect& area);

		bool fits(const Element& elm) const;
        void insert(const Element& elm);
        bool remove(const Element& elm);
        bool adjust(bool all=true);

        bool clear();
        void find(Elements& elms) const;
        bool empty() const;
        size_t size() const;

        bool clear(const gorn::RectMatch& match);
        void find(Elements& elms, const gorn::RectMatch& match) const;
        bool empty(const gorn::RectMatch& match) const;
        size_t size(const gorn::RectMatch& match) const;

        template<typename Filter>
        bool clear(Filter f);

        template<typename Filter>
        void find(Elements& elms, Filter f) const;

        template<typename Filter>
        bool empty(Filter f) const;
        
        template<typename Filter>
        size_t size(Filter f) const;

        size_t sizeNodes() const;

        std::vector<gorn::Rect> getElementsRects() const;
        std::vector<gorn::Rect> getNodesRects() const;
    };

    template<typename T>
    OcTreeNode<T>::OcTreeNode(const gorn::Rect& area, Node* parent,
            size_t max, const glm::vec3& divs):
        _area(area), _parent(parent),
        _maxElements(max), _divisions(divs),
        _matchType(MatchType::Overlap)
    {
    }

    template<typename T>
    const gorn::Rect& OcTreeNode<T>::getArea() const
    {
        return _area;
    }

    template<typename T>
    void OcTreeNode<T>::setArea(const gorn::Rect& area)
    {
        _area = area;
    }

	template<typename T>
	bool OcTreeNode<T>::fits(const Element& elm) const
	{
		return _area.matches(elm.getArea(), _matchType);
	}

    template<typename T>
    void OcTreeNode<T>::insert(const Element& elm)
    {
        for(auto& branch : _branches)
        {
            if(branch->remove(elm))
            {
                break;
            }
        }
        for(auto& branch : _branches)
        {
            if(branch->fits(elm))
            {
				branch->insert(elm);
                return;
            }
        }
		auto itr = std::find(_elements.begin(), _elements.end(), elm);
        if(itr == _elements.end())
        {
            _elements.insert(itr, elm);
        }
        else
        {
            *itr = elm;
        }
    }

    template<typename T>
    bool OcTreeNode<T>::remove(const Element& elm)
    {
        for(auto& branch : _branches)
        {
            if(branch->remove(elm))
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
        for(auto& branch : _branches)
        {
            if(branch->clear())
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
        for(auto& branch : _branches)
        {
            branch->find(elms);
        }
    }

    template<typename T>
    bool OcTreeNode<T>::empty() const
    {
        if(!_elements.empty())
        {
            return false;
        }
        for(auto& branch : _branches)
        {
            if(!branch->empty())
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
        for(auto& branch : _branches)
        {
            size += branch->size();
        }
        return size;
    }

    template<typename T>
    size_t OcTreeNode<T>::sizeNodes() const
    {
        size_t size = 1;
        for(auto& branch : _branches)
        {
            size += branch->sizeNodes();
        }
        return size;
    }

    template<typename T>
    bool OcTreeNode<T>::clear(const gorn::RectMatch& match)
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
        for(auto& branch : _branches)
        {
            if(branch->clear(match))
            {
                changed = true;
            }
        }
        return changed;
    }

    template<typename T>
    void OcTreeNode<T>::find(Elements& elms, const gorn::RectMatch& match) const
    {
        if(!match.matches(_area))
        {
            return;
        }
        for(auto& elm : _elements)
        {
            if (match.matches(elm.getArea()))
            {
                elms.push_back(elm);
            }
        }
        for(auto& branch : _branches)
        {
            branch->find(elms, match);
        }
    }

    template<typename T>
    bool OcTreeNode<T>::empty(const gorn::RectMatch& match) const
    {
        if(!match.matches(_area))
        {
            return true;
        }
        for(auto& elm : _elements)
        {
            if (match.matches(elm.getArea()))
            {
                return false;
            }
        }
        for(auto& branch : _branches)
        {
            if(!branch->empty(match))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    size_t OcTreeNode<T>::size(const gorn::RectMatch& match) const
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
        for(auto& branch : _branches)
        {
            size += branch->size(match);
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

        for(auto& branch : _branches)
        {
            if(branch->clear(filter))
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
        for(auto& elm : _elements)
        {
            if (filter(elm))
            {
                elms.push_back(elm);
            }
        }
        for(auto& branch : _branches)
        {
            branch->find(elms, filter);
        }
    }

    template<typename T>
    template<typename Filter>
    bool OcTreeNode<T>::empty(Filter filter) const
    {
        for(auto& elm : _elements)
        {
            if (filter(elm))
            {
                return false;
            }
        }
        for(auto& branch : _branches)
        {
            if(!branch->empty(filter))
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
        for(auto& branch : _branches)
        {
            size += branch->size(filter);
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
            for(auto& branch : _branches)
            {
                if(branch->split())
                {
                    changed =  true;
                }
            }
            return changed;
        }
        std::vector<glm::vec3> origins;
        for(auto& elm : _elements)
        {
            auto& p = elm.getArea().origin;
            auto itr = std::find(origins.begin(), origins.end(), p);
            if(itr == origins.end())
            {
                origins.insert(itr, p);
            }
        }
        if(origins.size() <= _maxElements)
        {
            return false;
        }
        auto parts = _area.divide(_divisions);
        _branches.reserve(parts.size());
        for(auto& part : parts)
        {
            _branches.push_back(std::unique_ptr<Node>(
                new Node(part, this, _maxElements, _divisions)));
        }
        _elements.erase(std::remove_if(_elements.begin(), _elements.end(),
            [this](Element& elm)
            {
                for(auto& branch : _branches)
                {
                    if(branch->fits(elm))
                    {
						branch->insert(elm);
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
        for(auto& branch : _branches)
        {
            if(branch->join())
            {
                return true;
            }
        }
        for(auto& branch : _branches)
        {
            if(branch->sizeNodes() > 1)
            {
                return false;
            }
            size += branch->_elements.size();
        }
        if(size > _maxElements)
        {
            return false;
        }
        for(auto& branch : _branches)
        {
            auto& elms = branch->_elements;
            _elements.insert(_elements.end(), elms.begin(), elms.end());
        }
        _branches.clear();
        return true;
    }

    template<typename T>
    bool OcTreeNode<T>::adjust(bool all)
    {
        if(all)
        {
            bool changed = false;
            while(adjust(false))
            {
                changed = true;
            }
            return changed;
        }
        else
        {
            if(join())
            {
                return true;
            }
            if(split())
            {
                return true;
            }
            return false;
        }
    }

    template<typename T>
    std::vector<gorn::Rect> OcTreeNode<T>::getElementsRects() const
    {
        std::vector<gorn::Rect> rects;
        rects.reserve(_elements.size());
        for(auto& elm : _elements)
        {
            rects.push_back(elm.getArea());
        }

        for(auto& branch : _branches)
        {
            auto brects = branch->getElementsRects();
            rects.insert(rects.end(), brects.begin(), brects.end());
        }
        return rects;
    }

    template<typename T>
    std::vector<gorn::Rect> OcTreeNode<T>::getNodesRects() const
    {
        std::vector<gorn::Rect> rects;
        rects.push_back(_area);

        for(auto& branch : _branches)
        {
            auto brects = branch->getNodesRects();
            rects.insert(rects.end(), brects.begin(), brects.end());
        }
        return rects;
    }

}

#endif

