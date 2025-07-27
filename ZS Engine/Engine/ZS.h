#pragma once
#include "../Common.h"
#include <string>

#define ZS_VERSION "0.0.1"

typedef class ZS
{
public:
	typedef struct Layer
	{
		std::string name;
		size_t id;
	}Layer;

	typedef class ConverterImGui
	{
	public:
		static sf::Vector2f ConvertImVec2toVector2f(const ImVec2& _vec);
		static ImVec2 ConvertVector2ftoImVec2(const sf::Vector2f& _vec);
	}ConverterImGui;

	typedef class GameObject
	{
	public:
		typedef struct Transform
		{
			sf::Vector2f pos;
			float rotation;
			sf::Vector2f scale;
		}Transform;

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

			typedef class Grid
			{
			public:
				typedef class Cell
				{
				public:
					size_t id;
					size_t line;
					size_t column;
					bool mouseInside;
					bool selected;
					sf::RectangleShape rect;
				}Cell;

				sf::Vector2i gridSize;
				sf::Vector2i cellSize;
				sf::Vector2f cellGap;

				std::vector<std::vector<Cell>> cellList;
			}Grid;

			typedef class Tilemap
			{
			public:
				typedef class Sprite : public Grid::Cell
				{
				public:
					sf::Sprite sprite;
				}sprite;

				sf::Color color;

				std::vector<std::vector<Sprite>> spriteList;

			}Tilemap;

			ComponentType type;
			void* data;
		}Component;

		std::vector<GameObject> child;

		std::string name;
		std::string* tag;
		Layer* layer;
		Transform transform;

		std::vector<Component> componentList;

	}GameObject;

	void Load();
	void PollEvent(sf::RenderWindow& _renderWindow, const sf::Event& _event);
	void Update(sf::RenderWindow& _renderWindow, float _dt);
	void Draw(sf::RenderWindow& _renderWindow);
	void Cleanup();

	sf::View sceneView;
	std::unique_ptr<sf::RenderTexture> sceneRender;

	std::vector<std::string> tagList;
	std::vector<Layer> layerList;

	GameObject* selectedGO;
	std::vector<GameObject> gameObjectList;

	sf::Texture cellTexture;
private:
	void Hierarchy();
	void Inspector();
	void Scene();
	void Tileset();
}ZS;