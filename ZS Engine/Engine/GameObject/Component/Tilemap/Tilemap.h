#pragma once
#include "../../Common.h"
#include "../IComponentData.h"
#include "../Grid/Grid.h"

namespace ZS
{
	typedef class Tilemap : public IComponentData
	{
	public:
		void PaintTiles(Grid::Cell* _selectedCell, sf::Texture& _tilesetTexture);
		void ChangeColor();

		sf::Color color;

		Grid grid;

	private:
		const std::string Name() const override;
	}Tilemap;
}
