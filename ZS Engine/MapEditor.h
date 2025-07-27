#pragma once

#include "Common.h"
#include <memory>

typedef class MapEditor
{
private:
	typedef class Cell
	{
	public:
		size_t id;
		size_t line;
		size_t column;

		bool mouseInside;
		bool selected;

		sf::Sprite spr;

		// Constructor
		Cell() : id(0), line(0), column(0), mouseInside(false), selected(false), spr()
		{
		}
	}Cell;


	typedef class Grid
	{
	public:
		void Load(sf::Texture& _cellTexture, size_t _width, size_t _height, size_t _cellSize = 32, const int* _startId = nullptr);

		void DragingAndZoomPollEvent(const sf::Event& _event, const sf::RenderWindow& _renderWindow, const std::unique_ptr<sf::RenderTexture>& _rTextutre);
		void SelectCellPollEvent(const sf::Event& _event, Cell* _selectedCell);

		void CellOverriding(sf::Vector2f _localPos);
		void Draging(sf::Vector2i _mousePos, std::unique_ptr<sf::RenderTexture>& _rTexture);

		void Draw(const std::unique_ptr<sf::RenderTexture>& _rTexture);

		// Constructor
		Grid() : view(), cellList(), isDragging(false)
		{
		}

		sf::View view;
		std::vector<std::vector<Cell>> cellList;

		bool isDragging;
		sf::Vector2i dragStartMousePixel;
		sf::Vector2f dragStartViewCenter;
	}Grid;

	typedef class Layers
	{
	public:
		typedef enum LayerType
		{
			LAYER_TYPE_NONE,
			LAYER_TYPE_TILES,
			LAYER_TYPE_OBJECTS,
			LAYER_TYPE_IMAGE,
		}LayerType;

		typedef class LayerSprite : public Cell
		{
		public:
			sf::Sprite sprite;
		}LayerSprite;

		typedef class LayerTile
		{
		public:

			void PaintTiles(Cell& _selectedCell, sf::Texture& _tilesetTexture);

			std::string name;
			sf::Vector2f pos;

			Grid grid;

			std::vector<LayerSprite> spriteList;
		}LayerTile;

		typedef class LayerObject
		{

		}LayerObject;

		LayerType layerType;
		bool selected;
		void* layer;
	}Layers;

	typedef class Tilesets
	{
	public:
		std::unique_ptr<sf::RenderTexture> tilesetInterface;
		sf::Sprite sprite;

		sf::Texture tilesetTexture;

		Grid grid;
		Cell selectedCell;

		Tilesets() : tilesetInterface(std::make_unique<sf::RenderTexture>()), sprite(), tilesetTexture(), grid(),
			selectedCell()
		{
		}

		Tilesets(const Tilesets& other) :
			tilesetInterface(std::make_unique<sf::RenderTexture>()),
			sprite(other.sprite),
			tilesetTexture(other.tilesetTexture),
			grid(other.grid),
			selectedCell()
		{
			if (other.tilesetInterface && other.tilesetInterface->getSize().x > 0 && other.tilesetInterface->getSize().y > 0)
			{
				tilesetInterface->create(other.tilesetInterface->getSize().x, other.tilesetInterface->getSize().y);
			}
		}

		Tilesets& operator=(const Tilesets& other)
		{
			if (this != &other) {
				tilesetInterface = std::make_unique<sf::RenderTexture>();
				sprite = other.sprite;
				tilesetTexture = other.tilesetTexture;
				grid = other.grid;
				selectedCell = other.selectedCell;
				if (other.tilesetInterface && other.tilesetInterface->getSize().x > 0 && other.tilesetInterface->getSize().y > 0)
				{
					tilesetInterface->create(other.tilesetInterface->getSize().x, other.tilesetInterface->getSize().y);
				}
			}
			return *this;
		}
	}Tilesets;

	void InitInterface(void);

	void UpdateLayers(sf::Vector2f _localPos);

	sf::Texture cellTexture;
	sf::Texture bgTexture;
	std::unique_ptr<sf::RenderTexture> sceneInterface;

	Grid worldGrid;
	Tilesets tilesets;

	std::vector<Layers> layers;

public:
	void Load(void);
	void PollEvent(sf::RenderWindow& _renderWindow, sf::Event _event);
	void Update(float const _dt, sf::RenderWindow& _renderWindow);
	void Draw(sf::RenderWindow& _renderWindow);

	sf::Vector2f ConvertImVec2toVector2f(ImVec2 _v);
	bool ScreenOverlap(sf::Vector2f _screenPos, sf::Vector2f _screenSize, sf::Vector2f _mousePos);
}MapEditor;