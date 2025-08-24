#include "Tilemap.h"

void ZS::Tilemap::PaintTiles(Grid::Cell* _selectedCell, sf::Texture& _tilesetTexture)
{
	for (auto& cellList : this->grid.cellList)
	{
		for (auto& cell : cellList)
		{
			if (cell.mouseInside && _selectedCell && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				cell.id = _selectedCell->id;

				cell.spr.setTexture(_tilesetTexture);
				cell.spr.setPosition(cell.rect.getPosition());
				sf::IntRect textureRect = sf::IntRect(
					_selectedCell->column * this->grid.cellSize.x,
					_selectedCell->line * this->grid.cellSize.y,
					this->grid.cellSize.x,
					this->grid.cellSize.y
				);
				cell.spr.setTextureRect(textureRect);
			}
		}
	}
}

void ZS::Tilemap::ChangeColor()
{
	for (auto& cellList : this->grid.cellList)
	{
		for (auto& cell : cellList)
		{
			cell.spr.setColor(this->color);
		}
	}
}

const std::string ZS::Tilemap::Name() const
{
	return "Tilemap";
}
