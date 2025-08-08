#pragma once
#include "../Common.h"
#include <string>
#include <functional>

#define ZS_VERSION "0.0.1"

typedef class ZS
{
public:
	typedef struct Layer
	{
		std::string name;
		size_t id;
	}Layer;

	typedef class ImGuiUtility
	{
	public:
		static sf::Vector2f ConvertImVec2toVector2f(const ImVec2& _vec);
		static ImVec2 ConvertVector2ftoImVec2(const sf::Vector2f& _vec);
	}ImGuiUtility;

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
					sf::Sprite spr;
				}Cell;

				sf::Vector2i gridSize;
				sf::Vector2i cellSize;
				sf::Vector2f cellGap;

				std::vector<std::vector<Cell>> cellList;

				void CellOverriding(sf::Vector2f _worldPos);
				void SelectCell(Cell** _selectedCell);
			}Grid;

			typedef class Tilemap
			{
			public:
				void PaintTiles(Grid::Cell* _selectedCell, sf::Texture& _tilesetTexture);

				sf::Color color;

				Grid grid;

			}Tilemap;

			ComponentType type;
			void* data;
		}Component;

		GameObject* parent;
		std::vector<GameObject*> child;

		std::string name;
		std::string* tag;
		Layer* layer;
		Transform transform;

		std::vector<Component> componentList;

	}GameObject;

	typedef class Tileset
	{
	public:
		GameObject::Component::Grid grid;

		sf::Texture texture;
		sf::Sprite sprite;
	}Tileset;

	void Load();
	void PollEvent(sf::RenderWindow& _renderWindow, const sf::Event& _event);
	void Update(sf::RenderWindow& _renderWindow, float _dt);
	void Draw(sf::RenderWindow& _renderWindow);
	void Cleanup();

	sf::View sceneView;
	std::unique_ptr<sf::RenderTexture> sceneRender;

	sf::View tilesetView;
	std::unique_ptr<sf::RenderTexture> tilesetRender;
	Tileset tileset;

	std::vector<std::string> tagList;
	std::vector<Layer> layerList;

	GameObject* selectedGO;
	std::vector<GameObject*> gameObjectList;

	sf::Texture cellTexture;

	GameObject* selectedTileMapGO;
	GameObject::Component::Grid::Cell* selectedCell;
	std::vector<GameObject*> tilemapList;

private:
	void HierarchyInterface();
	void InspectorInterface();
	void SceneInterface();
	void TilesetInterface();

	GameObject::Component* GetComponent(GameObject* const _gO, GameObject::Component::ComponentType _type);

	void UpdateGrid(GameObject& _gO, sf::Vector2f _worldPos);

	void ResizeGrid(GameObject& _gO, sf::Vector2i& _gridSize, sf::Vector2i& _cellSize, sf::Vector2f& _cellGap);
	std::vector<std::vector<GameObject::Component::Grid::Cell>> SaveGridID(GameObject::Component::Grid& _grid);
	void RestoreGridID(GameObject::Component::Grid& _grid, const std::vector<std::vector<GameObject::Component::Grid::Cell>>& _savedGridID);
}ZS;