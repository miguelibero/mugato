#ifndef __mugato__OcTreeElement__
#define __mugato__OcTreeElement__

#include <mugato/base/Rectangle.hpp>

namespace mugato
{
    template<typename T>
    class OcTreeElement
    {
    private:
        Rectangle _area;
        T _content;
    public:
        OcTreeElement(const Rectangle& area, const T& content);
        OcTreeElement(const Rectangle& area, T&& content);

        bool operator==(const OcTreeElement<T>& other) const;
        bool operator!=(const OcTreeElement<T>& other) const;

        const T& getContent() const;
        T& getContent();

        const Rectangle& getArea() const;
    };

    template<>
    class OcTreeElement<void>
    {
    private:
        Rectangle _area;
    public:
        OcTreeElement(const Rectangle& area):
        _area(area)
        {
        }

        bool operator==(const OcTreeElement<void>& other) const
        {
            return false;
        }

        bool operator!=(const OcTreeElement<void>& other) const
        {
            return true;
        }

        const Rectangle& getArea() const
        {
            return _area;
        }
    };

    template<typename T>
    OcTreeElement<T>::OcTreeElement(const Rectangle& area, const T& content):
    _area(area), _content(content)
    {
    }

    template<typename T>
    OcTreeElement<T>::OcTreeElement(const Rectangle& area, T&& content):
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
    bool OcTreeElement<T>::operator==(const OcTreeElement<T>& other) const
    {
        return _content == other._content;
    }

    template<typename T>
    bool OcTreeElement<T>::operator!=(const OcTreeElement<T>& other) const
    {
        return !(*this==other);
    }

    template<typename T>
    const Rectangle& OcTreeElement<T>::getArea() const
    {
        return _area;
    }

}

#endif

