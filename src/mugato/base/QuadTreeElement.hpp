#ifndef __mugato__QuadTreeElement__
#define __mugato__QuadTreeElement__

#include <mugato/base/Rectangle.hpp>

namespace mugato
{
    template<typename T>
    class QuadTreeElement
    {
    public:
        typedef Rectangle Area;
    private:
        Area _area;
        T _content;
    public:
        QuadTreeElement(const Area& area, const T& content);
        QuadTreeElement(const Area& area, T&& content);

        const T& getContent() const;
        T& getContent();

        const Area& getArea() const;
    };

    template<>
    class QuadTreeElement<void>
    {
    public:
        typedef Rectangle Area;
    private:
        Area _area;
    public:
        QuadTreeElement(const Area& area):
        _area(area)
        {
        }

        const Area& getArea() const
        {
            return _area;
        }
    };

    template<typename T>
    QuadTreeElement<T>::QuadTreeElement(const Area& area, const T& content):
    _area(area), _content(content)
    {
    }

    template<typename T>
    QuadTreeElement<T>::QuadTreeElement(const Area& area, T&& content):
    _area(area), _content(std::move(content))
    {
    }

    template<typename T>
    const T& QuadTreeElement<T>::getContent() const
    {
        return _content;
    }

    template<typename T>
    T& QuadTreeElement<T>::getContent()
    {
        return _content;
    }

    template<typename T>
    const typename QuadTreeElement<T>::Area&
        QuadTreeElement<T>::getArea() const
    {
        return _area;
    }

}

#endif

