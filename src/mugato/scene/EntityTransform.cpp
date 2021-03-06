#include <mugato/scene/EntityTransform.hpp>
#include <gorn/base/Ray.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace mugato
{
    EntityTransform::EntityTransform():
    _dirty(true),
    _position(0.0f),
    _rotation(0.0f),
    _scale(1.0f),
    _pivot(0.0f)
    {
    }

    const EntityTransform::Vector& EntityTransform::getPosition() const
    {
        return _position;
    }

    const EntityTransform::Vector& EntityTransform::getRotation() const
    {
        return _rotation;
    }

    const EntityTransform::Vector& EntityTransform::getScale() const
    {
        return _scale;
    }

    const EntityTransform::Vector& EntityTransform::getPivot() const
    {
        return _pivot;
    }

    const EntityTransform::Vector& EntityTransform::getSize() const
    {
        return _size;
    }

    const gorn::Rect& EntityTransform::getArea() const
    {
        return _area;
    }

    const gorn::Rect& EntityTransform::getLocalArea() const
    {
        return _localArea;
    }

    const EntityTransform::Matrix& EntityTransform::getMatrix() const
    {
        return _matrix;
    }

    const EntityTransform::Matrix& EntityTransform::getInverseMatrix() const
    {
        return _inverseMatrix;
    }

    void EntityTransform::setPosition(const Vector2& val)
    {
        setPosition(Vector(val.x, val.y, 0.0f));
    }

    void EntityTransform::setRotation(const Vector2& val)
    {
        setRotation(Vector(val.x, val.y, 0.0f));
    }

    void EntityTransform::setScale(const Vector2& val)
    {
        setScale(Vector(val.x, val.y, 1.0f));
    }

    void EntityTransform::setPivot(const Vector2& val)
    {
        setPivot(Vector(val.x, val.y, 0.0f));
    }

    void EntityTransform::setSize(const Vector2& val)
    {
        setSize(Vector(val.x, val.y, 0.0f));
    }

    void EntityTransform::setRotation(float val)
    {
        setRotation(glm::vec3(0.0f, val, 0.0f));
    }

    void EntityTransform::setScale(float val)
    {
        setScale(glm::vec3(val));
    }

    void EntityTransform::setPosition(const Vector& val)
    {
        if(_position != val)
        {
            _dirty = true;
            _position = val;
        }
    }

	void EntityTransform::setPositionX(float val)
	{
		if(_position.x != val)
		{
			_dirty = true;
			_position.x = val;
		}
	}

	void EntityTransform::setPositionY(float val)
	{
		if(_position.y != val)
		{
			_dirty = true;
			_position.y = val;
		}
	}

	void EntityTransform::setPositionZ(float val)
	{
		if(_position.z != val)
		{
			_dirty = true;
			_position.z = val;
		}
	}


    void EntityTransform::setRotation(const Vector& val)
    {
        if(_rotation != val)
        {
            _dirty = true;
            _rotation = val;
        }
    }

    void EntityTransform::setScale(const Vector& val)
    {
        if(_scale != val)
        {
            _dirty = true;
            _scale = val;
        }
    }

    void EntityTransform::setPivot(const Vector& val)
    {
        if(_pivot != val)
        {
            _dirty = true;
            _pivot = val;
        }
    }

    void EntityTransform::setSize(const Vector& val)
    {
        if(_size != val)
        {
            _dirty = true;
            _size = val;
        }
    }

	void EntityTransform::setMatrix(const Matrix& m)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(m, _scale, orientation, _position, skew, perspective);
		_rotation = glm::eulerAngles(orientation);
		_matrix = m;
		_dirty = false;
	}

    EntityTransform::Vector2
        EntityTransform::getLocalToParentPoint(const Vector2& p) const
    {
        return Vector2(_matrix*glm::vec4(p, 0.0, 1.0));
    }

    EntityTransform::Vector2
        EntityTransform::getParentToLocalPoint(const Vector2& p) const
    {
        return Vector2(_inverseMatrix*glm::vec4(p, 0.0, 1.0));
    }

    EntityTransform::Vector
        EntityTransform::getLocalToParentPoint(const Vector& p) const
    {
        return Vector(_matrix*glm::vec4(p, 1.0));
    }

    EntityTransform::Vector
        EntityTransform::getParentToLocalPoint(const Vector& p) const
    {
        return Vector(_inverseMatrix*glm::vec4(p, 1.0));
    }

    bool EntityTransform::update()
    {
        if(_dirty)
        {
            _matrix = glm::translate(glm::mat4(), _position)
                * glm::translate(glm::mat4(), _pivot)
                * glm::scale(glm::mat4(), _scale)
                * glm::orientate4(_rotation)
                * glm::translate(glm::mat4(), -_pivot)
                ;
            _inverseMatrix = glm::inverse(_matrix);
            _localArea = gorn::Rect(glm::vec3(0.0f), _size);
            _area = _localArea*_matrix;
            _dirty = false;
            return true;
        }
        return false;
    }

    EntityTransform& EntityTransform::operator=(const EntityTransform& other)
    {
        _position = other._position;
        _rotation = other._rotation;
        _scale = other._scale;
        _pivot = other._pivot;
        _size = other._size;
        _dirty = true;
        return *this;
    }

    bool EntityTransform::operator==(const EntityTransform& other) const
    {
        return _position == other._position
         && _rotation == other._rotation
         && _scale == other._scale
         && _pivot == other._pivot
         && _size == other._size;
    }

    bool EntityTransform::operator!=(const EntityTransform& other) const
    {
        return !(*this==other);
    }

}
