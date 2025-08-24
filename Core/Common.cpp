#include "Common.h"

sf::Sprite LoadSprite(const std::string _path, bool _isCentered)
{
	sf::Sprite tempSprite;
	sf::Texture* tempTex = new sf::Texture();

	tempTex->loadFromFile(_path);
	tempSprite.setTexture(*tempTex);

	if (_isCentered)
	{
		sf::Vector2u tempTaille = tempTex->getSize();
		sf::Vector2f tempOrigin = sf::Vector2f(tempTaille.x / 2, tempTaille.y / 2);
		tempSprite.setOrigin(tempOrigin);
	}

	return tempSprite;
}

void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window)
{
	_sprite.setRotation(_angle);
	_sprite.setPosition(_pos);
	_window.draw(_sprite);
}

void DrawPixel(sf::Image& _image, sf::Vector2i _pos, sf::Color _color)
{
	if (_pos.x >= 0 && _pos.x < _image.getSize().x
		&& _pos.y >= 0 && _pos.y < _image.getSize().y)
	{
		_image.setPixel(_pos.x, _pos.y, _color);
	}
}