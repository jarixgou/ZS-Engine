#include "GameObject.h"

std::vector<std::vector<size_t>> SaveCellID(const ZS::Grid& _grid);
void RestoreGridID(ZS::Grid& _grid, const std::vector<std::vector<size_t>>& _savedGridID, sf::Texture& _tilesetTexture);

const std::vector<ZS::Component*>* ZS::GameObject::GetComponentList() const
{
	return &this->componentList;
}

void ZS::GameObject::ResizeGrid(const Grid& _grid, sf::Texture& _cellTexture, sf::Texture& _tilesetTexture)
{
	if (auto* grid = this->GetComponent<Grid>())
	{
		grid->gridSize = _grid.gridSize;
		grid->cellSize = _grid.cellSize;
		grid->cellGap = _grid.cellGap;

		grid->DestroyGrid();
		grid->CreateGrid(_cellTexture, false);
	}

	if (auto* tilemap = this->GetComponent<Tilemap>())
	{
		tilemap->grid.gridSize = _grid.gridSize;
		tilemap->grid.cellSize = _grid.cellSize;
		tilemap->grid.cellGap = _grid.cellGap;

		auto savedGridID = SaveCellID(tilemap->grid);
		tilemap->grid.DestroyGrid();
		tilemap->grid.CreateGrid(_cellTexture, false);
		RestoreGridID(tilemap->grid, savedGridID, _tilesetTexture);
	}

	for (auto child : this->child)
	{
		child->ResizeGrid(_grid, _cellTexture, _tilesetTexture);
	}
}

std::vector<std::vector<size_t>> SaveCellID(const ZS::Grid& _grid)
{
	std::vector<std::vector<size_t>> cellID;
	cellID.resize(_grid.cellList.size());
	for (size_t line = 0; line < _grid.cellList.size(); ++line)
	{
		cellID[line].resize(_grid.cellList[line].size());
		for (size_t column = 0; column < _grid.cellList[line].size(); ++column)
		{
			cellID[line][column] = _grid.cellList[line][column].id;
		}
	}

	return cellID;
}

void RestoreGridID(ZS::Grid& _grid, const std::vector<std::vector<size_t>>& _savedGridID, sf::Texture& _tilesetTexture)
{
	for (size_t line = 0; line < _grid.cellList.size(); ++line)
	{
		for (size_t column = 0; column < _grid.cellList[line].size(); ++column)
		{
			if (line < _savedGridID.size() && column < _savedGridID[line].size())
			{
				ZS::Grid::Cell& cell = _grid.cellList[line][column];

				cell.id = _savedGridID[line][column];

				if (cell.id != 0)
				{
					cell.spr.setTexture(_tilesetTexture);

					int tileX = (cell.id - 1) % (_tilesetTexture.getSize().x / _grid.cellSize.x);
					int tileY = (cell.id - 1) / (_tilesetTexture.getSize().x / _grid.cellSize.x);

					sf::IntRect textureRect(tileX * _grid.cellSize.x, tileY * _grid.cellSize.y, _grid.cellSize.x, _grid.cellSize.y);
					cell.spr.setTextureRect(textureRect);
				}

				cell.spr.setPosition(cell.rect.getPosition());
			}
		}
	}
}