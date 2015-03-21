#ifndef __mugato__EntityTransform__
#define __mugato__EntityTransform__

#include <glm/glm.hpp>
#include <mugato/base/Rectangle.hpp>

namespace mugato
{
    class EntityTransform
    {
    public:
        typedef glm::vec3 Vector;
        typedef glm::vec2 Vector2;
        typedef glm::mat4 Matrix;
    private:
        Matrix _matrix;
        bool _dirty;
        Vector _position;
        Vector _rotation;
        Vector _scale;
        Vector _pivot;
        Vector _size;
        Rectangle _area;

    public:
        EntityTransform();

        const Vector& getPosition() const;
        const Vector& getRotation() const;
        const Vector& getScale() const;
        const Vector& getPivot() const;
        const Vector& getSize() const;

        const Rectangle& getArea() const;
        const Matrix& getMatrix() const;

        void setPosition(const Vector& val);
        void setRotation(const Vector& val);
        void setScale(const Vector& val);
        void setPivot(const Vector& val);
        void setSize(const Vector& val);

        void setPosition(const Vector2& val);
        void setRotation(const Vector2& val);
        void setScale(const Vector2& val);
        void setPivot(const Vector2& val);
        void setSize(const Vector2& val);

        void setRotation(float val);
        void setScale(float val);

        bool update();

    };
}

#endif
