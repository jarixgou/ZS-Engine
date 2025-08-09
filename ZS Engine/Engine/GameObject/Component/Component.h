#pragma once
#include <memory>
#include <string>

#include "IComponentData.h"

namespace ZS
{
	typedef class Component
	{
	public:
		typedef enum ComponentType
		{
			COMPONENT_TYPE_NONE,
			COMPONENT_TYPE_GRID,
			COMPONENT_TYPE_TILEMAP,
			COMPONENT_TYPE_SPRITE,
			COMPONENT_TYPE_ANIMATION,
			COMPONENT_TYPE_TEXT,
			COMPONENT_TYPE_COLLIDER,
			COMPONENT_TYPE_RIGIDBODY,
		}ComponentType;

		ComponentType type;
		std::unique_ptr<IComponentData> data;

		std::string GetComponentName() const;
	}Component;
}