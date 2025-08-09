#include "GameObject.h"

std::vector<std::vector<size_t>> SaveCellID(const ZS::Grid& _grid);
void RestoreGridID(ZS::Grid& _grid, const std::vector<std::vector<size_t>>& _savedGridID, sf::Texture& _tilesetTexture);

ZS::Component* ZS::GameObject::GetComponent(Component::ComponentType _type) const
{

	for (const auto component : this->componentList)
	{
		if (component->type == _type)
		{
			return component;
		}
	}

	for (const auto child : this->child)
	{
		Component* found = child->GetComponent(_type);
		if (found)
		{
			return found;
		}
	}

	return nullptr;
}

void ZS::GameObject::ResizeGrid(const Grid& _grid, sf::Texture& _cellTexture, sf::Texture& _tilesetTexture) const
{
	for (auto component : this->componentList)
	{
		if (component->type == Component::COMPONENT_TYPE_GRID)
		{
			Grid* gridData = dynamic_cast<Grid*>(component->data.get());
			if (gridData)
			{
				gridData->gridSize = _grid.gridSize;
				gridData->cellSize = _grid.cellSize;
				gridData->cellGap = _grid.cellGap;

				gridData->DestroyGrid();
				gridData->CreateGrid(_cellTexture, false);
			}
		}
		else if (component->type == Component::COMPONENT_TYPE_TILEMAP)
		{
			Tilemap* tilemapData = dynamic_cast<Tilemap*>(component->data.get());
			if (tilemapData)
			{
				tilemapData->grid.gridSize = _grid.gridSize;
				tilemapData->grid.cellSize = _grid.cellSize;
				tilemapData->grid.cellGap = _grid.cellGap;

				auto savedGridID = SaveCellID(tilemapData->grid);
				tilemapData->grid.DestroyGrid();
				tilemapData->grid.CreateGrid(_cellTexture, false);
				RestoreGridID(tilemapData->grid, savedGridID, _tilesetTexture);
			}
		}
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