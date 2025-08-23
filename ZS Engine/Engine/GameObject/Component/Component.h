#pragma once
#include <memory>
#include <string>

#include "IComponentData.h"

namespace ZS
{
	inline const char* kAvailableComponents[] = { "Grid", "Tilemap" };
	
	typedef class Component
	{
	public:
		std::unique_ptr<IComponentData> data;

		std::string GetComponentName() const;
	}Component;
}