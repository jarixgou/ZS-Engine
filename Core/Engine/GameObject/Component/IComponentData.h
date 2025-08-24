#pragma once

typedef class IComponentData
{
public:
	virtual ~IComponentData() = default;
	virtual const std::string Name() const = 0;
}IComponentData;
