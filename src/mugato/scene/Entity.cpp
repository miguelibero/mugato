#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>
#include <mugato/scene/ActionComponent.hpp>
#include <gorn/base/Ray.hpp>
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

	glm::mat4 Entity::getModelInverse() const
	{
		return glm::inverse(getModelMatrix());
	}

    std::shared_ptr<Entity> Entity::getSharedPtr()
    {
        return shared_from_this();
    }

    void Entity::updateTransform()
    {
		if(!_transform.update())
		{
			return;
		}
        for(auto& elm : _components)
        {
			elm.component->onEntityTransformChanged(*this);
        }
        if(auto parent = _parent.lock())
        {
            parent->onChildTransformChanged(*this);
        }
		for(auto& child : _children)
		{
			child->onParentTransformChanged(*this);
		}
    }

    void Entity::onChildTransformChanged(Entity& child)
    {
        for(auto& elm : _components)
        {
			elm.component->onEntityChildTransformChanged(*this, child);
        }
		if (auto parent = _parent.lock())
		{
			parent->onChildTransformChanged(child);
		}
    }

	void Entity::onParentTransformChanged(Entity& parent)
	{
		for(auto& elm : _components)
		{
			elm.component->onEntityParentTransformChanged(*this, parent);
		}
		for (auto& child : _children)
		{
			child->onParentTransformChanged(parent);
		}
	}
	
	bool Entity::onScreenTouched(const gorn::RenderCamera& cam, const glm::vec2& p, TouchPhase phase)
    {
		for(auto& elm : _components)
		{
			if(elm.component->onScreenTouched(*this, cam, p, phase))
			{
				return true;
			}
		}
		for(auto itr = _children.rbegin(); itr != _children.rend(); ++itr)
		{
			auto& child = *itr;
			if(cam.matchesLayers(child->getLayers()))
			{
				if(child->onScreenTouched(cam, p, phase))
				{
					return true;
				}
			}
		}
		return false;
    }

    void Entity::addPendingComponents()
    {
		bool wereEmpty = _componentsToAdd.empty();
		_components.reserve(
			_components.size() + _componentsToAdd.size());
        for(auto& elm : _componentsToAdd)
        {
			elm.component->onAddedToEntity(*this);
			_components.push_back(std::move(elm));
        }
		_componentsToAdd.clear();
        if(!wereEmpty)
        {
            for(auto& elm : _components)
            {
				elm.component->onEntityComponentsLoaded(*this);
            }
        }
    }

	void Entity::removePendingComponents()
	{
		auto comps = _removedComponents;
		_removedComponents.clear();
		auto itr = std::remove_if(_components.begin(), _components.end(),
			[&comps](const ComponentData& elm) {
			return std::find(comps.begin(), comps.end(), elm.component.get()) != comps.end();
		});
		_components.erase(itr, _components.end());
	}

	void Entity::updateRemovedChildren()
	{
		auto& rs = _removedChildren;
		auto itr = std::remove_if(_children.begin(), _children.end(), [&rs]
			(const std::shared_ptr<Entity>& child) {
				return std::find(rs.begin(), rs.end(), child) != rs.end();
		});
		_children.erase(itr, _children.end());
		_removedChildren.clear();
	}

    void Entity::update(double dt)
    {
		removePendingComponents();
		updateRemovedChildren();
		updateTransform();
        addPendingComponents();
        for(auto& elm : _components)
        {
			elm.component->update(dt);
        }
        for(auto& child : _children)
        {
            child->update(dt);
        }
    }

    void Entity::fixedUpdate(double dt)
    {
        for(auto& elm : _components)
        {
			elm.component->fixedUpdate(dt);
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
			.withLayersAction(gorn::RenderStackAction::Push);
		queue.addCommand()
            .withBounding(_transform.getArea())
            .withTransformAction(gorn::RenderTransformStackAction::PushCheckpoint);
        queue.addCommand()
            .withTransform(_transform.getMatrix())
            .withTransformAction(gorn::RenderTransformStackAction::PushLocal);
        for(auto& elm : _components)
        {
			elm.component->render(queue);
        }
		for (auto& elm : _components)
		{
			elm.component->beforeEntityChildrenRender(queue);
		}
        for(auto& child : _children)
        {
            child->render(queue);
        }
		for (auto& elm : _components)
		{
			elm.component->afterEntityChildrenRender(queue);
		}
        queue.addCommand()
            .withBoundingAction(gorn::RenderStackAction::Pop)
            .withTransformAction(gorn::RenderTransformStackAction::PopCheckpoint);
		queue.addCommand()
			.withLayersAction(gorn::RenderStackAction::Pop);
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
        for (auto& elm : _components)
        {
            elm.component->onEntityChildAdded(*this, *child);
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
		auto itr = std::find(c.begin(), c.end(), child);
		if (itr == c.end())
		{
			return false;
		}
		_removedChildren.push_back(*itr);
        for (auto& elm : _components)
        {
			elm.component->onEntityChildRemoved(*this, *child);
        }
        return true;
    }

    Component& Entity::addComponent(Component::type_t type, std::unique_ptr<Component> comp)
    {
        auto ptr = comp.get();
        ptr->onAssignedToContext(getContext());
		_componentsToAdd.push_back(ComponentData{
			type, std::move(comp)
		});
        return *ptr;
    }

	bool Entity::hasComponent(Component::type_t type) const
	{
		for(auto& elm : _components)
		{
			if(elm.id == type)
			{
				return true;
			}
		}
		return false;
	}

	Component& Entity::getComponent(Component::type_t type)
	{
		for(auto& elm : _components)
		{
			if(elm.id == type)
			{
				return *elm.component;
			}
		}
		throw new Exception("Could not find component.");
	}

	const Component& Entity::getComponent(Component::type_t type) const
	{
		for (auto& elm : _components)
		{
			if (elm.id == type)
			{
				return *elm.component;
			}
		}
		throw new Exception("Could not find component.");
	}

	std::vector<Component*> Entity::getComponents(Component::type_t type)
	{
		std::vector<Component*> comps;
		for (auto& elm : _components)
		{
			if (elm.id == type)
			{
				comps.push_back(elm.component.get());
			}
		}
		return comps;
	}

	std::vector<const Component*> Entity::getComponents(Component::type_t type) const
	{
		std::vector<const Component*> comps;
		for (auto& elm : _components)
		{
			if (elm.id == type)
			{
				comps.push_back(elm.component.get());
			}
		}
		return comps;
	}

	bool Entity::removeComponent(const Component& comp)
	{
		for (auto& elm : _components)
		{
			if (elm.component.get() == &comp)
			{
				_removedComponents.push_back(&comp);
				return true;
			}
		}
		return false;
	}

	bool Entity::removeComponents(Component::type_t type)
	{
		bool found = false;
		for(auto& elm : _components)
		{
			if(elm.id == type)
			{
				found = true;
				_removedComponents.push_back(elm.component.get());
			}
		}
		return found;
	}

	Action& Entity::addAction(double duration, std::unique_ptr<Action> action)
	{
		auto ptr = action.get();
		addComponent<ActionComponent>(duration, std::move(action));
		return *ptr;
	}

	void Entity::clearActions()
	{
		removeComponents<ActionComponent>();
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

	void Entity::setLayers(const Layers& layers)
	{
		if(_layers != layers)
		{
			_layers = layers;
			for (auto& elm : _components)
			{
				elm.component->onEntityLayersChanged(*this);
			}
		}
	}

	void Entity::setLayer(int layer)
	{
		setLayers({ layer });
	}

	void Entity::clearLayers()
	{
		setLayers({});
	}

	bool Entity::hitBy(const gorn::Ray& ray) const
	{
		auto localRay = ray.transform(getModelInverse());
		return localRay.hits(getTransform().getLocalArea());
	}

	bool Entity::hitBy(const gorn::Ray& ray, glm::vec3& hp) const
	{
		auto localRay = ray.transform(getModelInverse());
		return localRay.hits(getTransform().getLocalArea(), hp);
	}
}
