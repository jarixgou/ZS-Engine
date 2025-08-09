#include "Component.h"

std::string ZS::Component::GetComponentName() const
{
	switch (this->type)
	{
	case ZS::Component::COMPONENT_TYPE_NONE:
		return "None";
	case ZS::Component::COMPONENT_TYPE_GRID:
		return "Grid";
	case ZS::Component::COMPONENT_TYPE_TILEMAP:
		return "Tilemap";
	case ZS::Component::COMPONENT_TYPE_SPRITE:
		return "Sprite";
	case ZS::Component::COMPONENT_TYPE_ANIMATION:
		return "Animation";
	case ZS::Component::COMPONENT_TYPE_TEXT:
		return "Text";
	case ZS::Component::COMPONENT_TYPE_COLLIDER:
		return "Collider";
	case ZS::Component::COMPONENT_TYPE_RIGIDBODY:
		return "Rigidbody";
	default:
		return "Unknown Component";
	}
}
