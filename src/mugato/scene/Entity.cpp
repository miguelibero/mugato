#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <algorithm>

namespace mugato
{
    Entity::Entity():
    _ctx(nullptr)
    {
    }

    Entity::~Entity()
    {
        (void)0;
    }

    void Entity::setContext(Context& ctx)
    {
        _ctx = &ctx;
    }

    Context& Entity::getContext() const
    {
        if(_ctx)
        {
            return *_ctx;
        }
        else if(hasParent())
        {
            return getParent().getContext();
        }
        else
        {
            throw Exception("Could not find context");
        }
    }

    const Entity::Transform& Entity::getTransform() const
    {
        return _transform;
    }

    Entity::Transform& Entity::getTransform()
    {
        return _transform;
    }

	glm::mat4 Entity::getModelMatrix() const
	{
		auto entity = this;
		glm::mat4 trans;
		while(true)
		{
			trans = trans * entity->getTransform().getMatrix();
			if(!entity->hasParent())
			{
				break;
			}
			entity = &entity->getParent();
		}
		return trans;
	}

    std::shared_ptr<Entity> Entity::getSharedPtr()
    {
        return shared_from_this();
    }

    void Entity::updateTransform()
    {
        if(_transform.update())
        {
            for(auto& comp : _components)
            {
                comp->onEntityTransformChanged(*this);
            }
            if(auto parent = _parent.lock())
            {
                parent->onChildTransformChanged(*this);
            }
        }
    }

    void Entity::onChildTransformChanged(Entity& child)
    {
        auto ptr = child.getSharedPtr();
        for(auto& comp : _components)
        {
            comp->onEntityChildTransformChanged(*this, ptr);
        }
    }

    Entity::TouchPhase Entity::touchChild(const glm::vec3& p,
        TouchPhase phase, const std::shared_ptr<Entity>& child)
    {
        auto itr = std::find_if(_touchedChildren.begin(),
            _touchedChildren.end(), [&child](std::weak_ptr<Entity>& c){
            return c.lock() == child;
        });
        auto contained = child->getTransform().getArea().contains(p);
        auto ending = phase == TouchPhase::End || phase == TouchPhase::Cancel;
        if(itr == _touchedChildren.end())
        {
            if(!contained || ending)
            {
                return TouchPhase::None;
            }
            else
            {
                _touchedChildren.push_back(child);
                return TouchPhase::Begin;
            }
        }
        else
        {
            if(contained && !ending)
            {
                return TouchPhase::Move;
            }
            else
            {
                _touchedChildren.erase(itr);
                if(phase == TouchPhase::End)
                {
                    return TouchPhase::End;
                }
                else
                {
                    return TouchPhase::Cancel;
                }
            }
        }
        return TouchPhase::None;
    }

    bool Entity::touch(const glm::vec3& p, TouchPhase phase)
    {
        auto ep = _transform.getParentToLocalPoint(p);
        bool handled = false;
        if(!handled)
        {
            for(auto itr = _children.rbegin(); itr != _children.rend(); ++itr)
            {
                auto& child = *itr;
                auto cphase = touchChild(ep, phase, child);
                if(cphase != TouchPhase::None)
                {
                    if(child->touch(ep, cphase))
                    {
                        handled = true;
                        phase = TouchPhase::Cancel;
                    }
                }
            }
        }
        if(!handled)
        {
            for(auto& comp : _components)
            {
                if(comp->onEntityTouched(*this, ep, phase))
                {
                    handled = true;
                }
            }
        }
        return handled;
    }

    void Entity::addPendingComponents()
    {
		bool wereEmpty = _componentsToAdd.empty();
		_components.reserve(
			_components.size() + _componentsToAdd.size());
        for(auto& comp : _componentsToAdd)
        {
            comp->onAddedToEntity(*this);
			_components.push_back(std::move(comp));
        }
		_componentsToAdd.clear();
        if(!wereEmpty)
        {
            for(auto& comp : _components)
            {
                comp->onEntityComponentsLoaded(*this);
            }
        }
    }

    void Entity::update(double dt)
    {
        updateTransform();
        addPendingComponents();
        for(auto& comp : _components)
        {
            comp->update(dt);
        }
        for(auto& child : _children)
        {
            child->update(dt);
        }
    }

    void Entity::fixedUpdate(double dt)
    {
        for(auto& comp : _components)
        {
            comp->fixedUpdate(dt);
        }
        for(auto& child : _children)
        {
            child->fixedUpdate(dt);
        }
    }

    void Entity::render(gorn::RenderQueue& queue)
    {
		queue.addCommand()
			.withLayers(_layers)
			.withLayersMode(gorn::RenderCommand::LayersMode::Start);
		queue.addCommand()
            .withBounding(_transform.getArea(),
                gorn::RenderCommand::BoundingMode::Start)
            .withTransformMode(gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
            .withTransform(_transform.getMatrix())
            .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal);
        for(auto& comp : _components)
        {
            comp->render(queue);
        }
		for (auto& comp : _components)
		{
			comp->beforeEntityChildrenRender(queue);
		}
        for(auto& child : _children)
        {
            child->render(queue);
        }
		for (auto& comp : _components)
		{
			comp->afterEntityChildrenRender(queue);
		}
        queue.addCommand()
            .withBoundingMode(gorn::RenderCommand::BoundingMode::End)
            .withTransformMode(gorn::RenderCommand::TransformMode::PopCheckpoint);
		queue.addCommand()
			.withLayersMode(gorn::RenderCommand::LayersMode::End);
    }

    std::shared_ptr<Entity> Entity::addChild(const std::shared_ptr<Entity>& child)
    {
        if(child == nullptr)
        {
            return addChild(std::make_shared<Entity>());
        }
        updateTransform();
        child->updateTransform();
        child->_parent = getSharedPtr();
        _children.push_back(child);
        for (auto& comp : _components)
        {
            comp->onEntityChildAdded(*this, child);
        }
        return child;
    }

    bool Entity::removeFromParent()
    {
        if(auto parent = _parent.lock())
        {
            return parent->removeChild(getSharedPtr());
        }
        return false;
    }

    bool Entity::removeChild(const std::shared_ptr<Entity>& child)
    {
        auto& c = _children;
        auto itr = std::remove_if(c.begin(), c.end(),
            [child](std::shared_ptr<Entity>& c){
                return c == child;
        });
        if(itr == c.end())
        {
            return false;
        }
        c.erase(itr, c.end());
        for (auto& comp : _components)
        {
            comp->onEntityChildRemoved(*this, child);
        }
        return true;
    }

    Component& Entity::addComponent(std::unique_ptr<Component> comp)
    {
        auto ptr = comp.get();
        ptr->onAssignedToContext(getContext());
        _componentsToAdd.push_back(std::move(comp));
        return *ptr;
    }

    bool Entity::hasParent() const
    {
        return !_parent.expired();
    }

    Entity& Entity::getParent()
    {
        return *_parent.lock();
    }

    const Entity& Entity::getParent() const
    {
        return *_parent.lock();
    }

    const Entity::Children& Entity::getChildren() const
    {
        return _children;
    }

	const Entity::Layers& Entity::getLayers() const
	{
		return _layers;
	}

	Entity::Layers& Entity::getLayers()
	{
		return _layers;
	}
}
