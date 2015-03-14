#ifndef __mugato__OcTreeElement__
#define __mugato__OcTreeElement__

#include <mugato/base/Rectangle.hpp>

namespace mugato
{
    template<typename T>
    class OcTreeElement
    {
    public:
        typedef Rectangle Area;
    private:
        Area _area;
        T _content;
    public:
        OcTreeElement(const Area& area, const T& content);
        OcTreeElement(const Area& area, T&& content);

        const T& getContent() const;
        T& getContent();

        const Area& getArea() const;
    };

    template<>
    class OcTreeElement<void>
    {
    public:
        typedef Rectangle Area;
    private:
        Area _area;
    public:
        OcTreeElement(const Area& area):
        _area(area)
        {
        }

        const Area& getArea() const
        {
            return _area;
        }
    };

    template<typename T>
    OcTreeElement<T>::OcTreeElement(const Area& area, const T& content):
    _area(area), _content(content)
    {
    }

    template<typename T>
    OcTreeElement<T>::OcTreeElement(const Area& area, T&& content):
    _area(area), _content(std::move(content))
    {
    }

    template<typename T>
    const T& OcTreeElement<T>::getContent() const
    {
        return _content;
    }

    template<typename T>
    T& OcTreeElement<T>::getContent()
    {
        return _content;
    }

    template<typename T>
    const typename OcTreeElement<T>::Area&
        OcTreeElement<T>::getArea() const
    {
        return _area;
    }

}

#endif

