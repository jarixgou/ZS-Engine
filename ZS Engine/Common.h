#ifndef COMMON__H
#define COMMON__H

#include <iostream>
#include <time.h>

#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include "SFML/OpenGL.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"

#define GAME_SCALE 3
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

sf::Sprite LoadSprite(const std::string _path , bool);
void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window);
void DrawPixel(sf::Image& _image, sf::Vector2i _pos, sf::Color _color);

#endif
