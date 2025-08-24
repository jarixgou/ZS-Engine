#include "Grid.h"

ZS::Grid::Cell::Cell(size_t _line, size_t _column, const sf::Vector2i& _size, const sf::Vector2f& _gap, sf::Texture& _texture) : line(_line), column(_column), mouseInside(false), selected(false)
{
	this->rect.setSize(static_cast<sf::Vector2f>(_size));
	this->rect.setTexture(&_texture);
	this->rect.setPosition(
		static_cast<float>(_column * (_size.x + _gap.x)),
		static_cast<float>(_line * (_size.y + _gap.y))
	);

}

void ZS::Grid::CellOverriding(sf::Vector2f _worldPos)
{
	for (auto& cellList : this->cellList)
	{
		for (auto& cell : cellList)
		{
			if (cell.rect.getGlobalBounds().contains(_worldPos))
			{
				cell.mouseInside = true;
				cell.rect.setFillColor(sf::Color::Red);
			}
			else
			{
				cell.mouseInside = false;
				cell.rect.setFillColor(sf::Color::White);
			}

			if (cell.selected)
			{
				cell.rect.setFillColor(sf::Color::Green);
			}
		}
	}
}

void ZS::Grid::SelectCell(Cell** _selectedCell)
{
	for (auto& cellList : this->cellList)
	{
		for (auto& cell : cellList)
		{
			if (cell.mouseInside && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				cell.selected = true;
				if (*_selectedCell)
				{
					if ((*_selectedCell)->selected)
					{
						(*_selectedCell)->selected = false;
					}
				}
				*_selectedCell = &cell;
			}
		}
	}
}

void ZS::Grid::CreateGrid(sf::Texture& _cellTexture, bool _giveID)
{
	this->cellList.resize(this->gridSize.y);
	for (int line = 0; line < this->gridSize.y; ++line)
	{
		this->cellList[line].resize(this->gridSize.x);
		for (int column = 0; column < this->gridSize.x; ++column)
		{
			this->cellList[line][column] = Cell(line, column, this->cellSize, this->cellGap, _cellTexture);
			this->cellList[line][column].id = _giveID ? (line * this->gridSize.x + column + 1) : 0;
		}
	}
}

void ZS::Grid::DestroyGrid()
{
	for (auto& cellList : this->cellList)
	{
		cellList.clear();
	}
	this->cellList.clear();

}

const std::string ZS::Grid::Name() const
{
	return "Grid";
}
