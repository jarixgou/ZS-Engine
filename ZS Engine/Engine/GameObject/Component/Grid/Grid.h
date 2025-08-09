#pragma once
#include "../../Common.h"
#include "../IComponentData.h"

namespace ZS
{
	typedef class Grid : public IComponentData
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

			Cell() = default;
			Cell(size_t _line, size_t _column, const sf::Vector2i& _size, const sf::Vector2f& _gap, sf::Texture& _texture);
		}Cell;

		sf::Vector2i gridSize;
		sf::Vector2i cellSize;
		sf::Vector2f cellGap;

		std::vector<std::vector<Cell>> cellList;

		void CellOverriding(sf::Vector2f _worldPos);
		void SelectCell(Cell** _selectedCell);

		void CreateGrid(sf::Texture& _cellTexture, bool _giveID);
		void DestroyGrid();
	}Grid;

}
