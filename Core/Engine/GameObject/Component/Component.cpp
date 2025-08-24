#include "Component.h"

std::string ZS::Component::GetComponentName() const
{
	return data ? data->Name() : "Unknown";
}
