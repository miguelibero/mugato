#ifndef __mugato__OcTreeElement__
#define __mugato__OcTreeElement__

#include <gorn/base/Rect.hpp>

namespace mugato
{
    template<typename T>
    class OcTreeElement
    {
    private:
        gorn::Rect _area;
        T _content;
    public:
        OcTreeElement(const gorn::Rect& area, const T& content);
        OcTreeElement(const gorn::Rect& area, T&& content);

        bool operator==(const OcTreeElement<T>& other) const;
        bool operator!=(const OcTreeElement<T>& other) const;

        const T& getContent() const;
        T& getContent();

        const gorn::Rect& getArea() const;
    };

    template<>
    class OcTreeElement<void>
    {
    private:
        gorn::Rect _area;
    public:
        OcTreeElement(const gorn::Rect& area):
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

        const gorn::Rect& getArea() const
        {
            return _area;
        }
    };

    template<typename T>
    OcTreeElement<T>::OcTreeElement(const gorn::Rect& area, const T& content):
    _area(area), _content(content)
    {
    }

    template<typename T>
    OcTreeElement<T>::OcTreeElement(const gorn::Rect& area, T&& content):
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
    const gorn::Rect& OcTreeElement<T>::getArea() const
    {
        return _area;
    }

}

#endif

