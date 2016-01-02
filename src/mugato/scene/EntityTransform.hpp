#ifndef __mugato__EntityTransform__
#define __mugato__EntityTransform__

#include <glm/glm.hpp>
#include <gorn/base/Rect.hpp>

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
        Matrix _inverseMatrix;
        bool _dirty;
        Vector _position;
        Vector _rotation;
        Vector _scale;
        Vector _pivot;
        Vector _size;
        gorn::Rect _area;
        gorn::Rect _localArea;

    public:
        EntityTransform();

        const Vector& getPosition() const;
        const Vector& getRotation() const;
        const Vector& getScale() const;
        const Vector& getPivot() const;
        const Vector& getSize() const;

        const gorn::Rect& getArea() const;
        const gorn::Rect& getLocalArea() const;
        const Matrix& getMatrix() const;
        const Matrix& getInverseMatrix() const;

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

        EntityTransform& operator=(const EntityTransform& other);
        bool operator==(const EntityTransform& other) const;
        bool operator!=(const EntityTransform& other) const;

        Vector2 getLocalToParentPoint(const Vector2& val) const;
        Vector2 getParentToLocalPoint(const Vector2& val) const;
        Vector getLocalToParentPoint(const Vector& val) const;
        Vector getParentToLocalPoint(const Vector& val) const;

    };
}

#endif
