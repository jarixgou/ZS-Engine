#include "Math.h"

sf::Vector2f CalculateVector(sf::Vector2f _a, sf::Vector2f _b)
{
	return { _b.x - _a.x, _b.y - _a.y };
}

float CrossProduct(sf::Vector2f _a, sf::Vector2f _b)
{
	return (_a.x * _b.y) - (_a.y * _b.x);
}
