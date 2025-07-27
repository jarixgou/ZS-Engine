#include "MapEditor.h"

void MapEditor::Grid::Load(sf::Texture& _cellTexture, size_t _width, size_t _height, size_t _cellSize, const int* _startId)
{
	size_t id = 0;

	if (_startId)
	{
		id = *_startId;
	}

	for (size_t y = 0; y < _height; ++y)
	{
		std::vector<Cell> tempCellList;
		for (size_t x = 0; x < _width; ++x)
		{
			Cell tempCell;

			tempCell.id = id;
			tempCell.line = y;
			tempCell.column = x;
			tempCell.selected = false;
			tempCell.spr.setTexture(_cellTexture);
			tempCell.spr.setPosition(static_cast<float>(x * _cellSize), static_cast<float>(y * _cellSize));

			tempCellList.push_back(tempCell);
			
			id++;
		}

		this->cellList.push_back(tempCellList);
	}

}

void MapEditor::Grid::DragingAndZoomPollEvent(const sf::Event& _event, const sf::RenderWindow& _renderWindow, const std::unique_ptr<sf::RenderTexture>& _rTextutre)
{
	if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Middle)
	{
		this->isDragging = true;
		this->dragStartMousePixel = sf::Mouse::getPosition(_renderWindow);
		this->dragStartViewCenter = this->view.getCenter();
	}

	if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Middle)
	{
		this->isDragging = false;
	}

	if (_event.type == sf::Event::MouseWheelScrolled)
	{
		if (_event.mouseWheelScroll.delta > 0)
		{
			this->view.zoom(0.9f);
		}
		else if (_event.mouseWheelScroll.delta < 0)
		{
			this->view.zoom(1.1f);
		}
		_rTextutre->setView(this->view);
	}
}

void MapEditor::Grid::SelectCellPollEvent(const sf::Event& _event, Cell* _selectedCell)
{
	if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left)
	{
		for (std::vector<Cell>& cellList : this->cellList)
		{
			for (Cell& cell : cellList)
			{
				if (cell.mouseInside)
				{
					cell.selected = !cell.selected;

					if (_selectedCell)
					{
						*_selectedCell = cell;
					}
				}
				else if (!cell.mouseInside && cell.selected)
				{
					cell.selected = !cell.selected;
				}
			}
		}
	}
}

void MapEditor::Grid::CellOverriding(sf::Vector2f _localPos)
{
	if (!this->isDragging)
	{
		for (std::vector<Cell>& cellList : this->cellList)
		{
			for (Cell& cell : cellList)
			{
				sf::FloatRect cellBounds = cell.spr.getGlobalBounds();

				if (cellBounds.contains(_localPos.x, _localPos.y))
				{
					cell.mouseInside = true;

					cell.spr.setColor(sf::Color::Red);
				}
				else
				{
					cell.mouseInside = false;

					cell.spr.setColor(sf::Color::White);
				}
			}
		}
	}
}

void MapEditor::Grid::Draging(sf::Vector2i _mousePos, std::unique_ptr<sf::RenderTexture>& _rTexture)
{
	if (this->isDragging)
	{
		sf::Vector2f deltaWorld = static_cast<sf::Vector2f>(this->dragStartMousePixel - _mousePos);

		sf::Vector2f newCenter = this->dragStartViewCenter + deltaWorld;

		this->view.setCenter(newCenter);
		_rTexture->setView(this->view);
	}
}

void MapEditor::Grid::Draw(const std::unique_ptr<sf::RenderTexture>& _rTexture)
{
	for (const std::vector<Cell>& cellList : this->cellList)
	{
		for (const Cell& cell : cellList)
		{
			_rTexture->draw(cell.spr);
		}
	}
}

void MapEditor::Layers::LayerTile::PaintTiles(Cell& _selectedCell, sf::Texture& _tilesetTexture)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (std::vector<Cell>& cellList : this->grid.cellList)
		{
			for (Cell cell : cellList)
			{
				if (cell.mouseInside)
				{
					bool sameId = false;
					bool modifiedSprite = false;

					sf::IntRect textureRect = sf::IntRect(_selectedCell.column * 32, _selectedCell.line * 32, 32, 32);


					LayerSprite layerSpriteTemp;
					layerSpriteTemp.sprite.setTexture(_tilesetTexture);
					layerSpriteTemp.sprite.setTextureRect(textureRect);
					layerSpriteTemp.sprite.setPosition(cell.spr.getPosition());

					layerSpriteTemp.id = _selectedCell.id;
					layerSpriteTemp.line = cell.line;
					layerSpriteTemp.column = cell.column;

					for (LayerSprite& layerSprite : this->spriteList)
					{
						if (cell.id != _selectedCell.id
							&& cell.line == layerSprite.line && cell.column == layerSprite.column)
						{
							modifiedSprite = true;
							cell.id = _selectedCell.id;
							layerSprite.sprite.setTextureRect(textureRect);
						}
						else if (cell.line == layerSprite.line && cell.column == layerSprite.column
							&& cell.id == _selectedCell.id)
						{
							sameId = true;
						}
					}

					if (!sameId && !modifiedSprite)
					{
						this->spriteList.push_back(layerSpriteTemp);
					}
				}
			}
		}
	}
}

void MapEditor::InitInterface()
{
	/*--- Scene Interface ---*/
	this->sceneInterface = std::make_unique<sf::RenderTexture>();
	this->sceneInterface->create(SCREEN_WIDTH, SCREEN_HEIGHT);

	/*--- Tileset Interface ---*/
	this->tilesets.tilesetInterface = std::make_unique<sf::RenderTexture>();
	this->tilesets.tilesetInterface->create(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void MapEditor::UpdateLayers(sf::Vector2f _localPos)
{
	for (Layers& layer : this->layers)
	{
		if (layer.layerType == Layers::LAYER_TYPE_TILES)
		{
			Layers::LayerTile* layerTile = static_cast<Layers::LayerTile*>(layer.layer);

			layerTile->grid.CellOverriding(_localPos);

			layerTile->PaintTiles(this->tilesets.selectedCell, this->tilesets.tilesetTexture);
		}
	}
}

void MapEditor::Load()
{
	MessageBoxA(NULL, "The tile map editor don't work, only the interface", "Map editor", MB_OK | MB_ICONEXCLAMATION);

	//LoadMapInfo();

	InitInterface();

	// Init grid
	this->cellTexture.loadFromFile("Assets/Map editor/Sprites/Cell.png");

	// World grid
	this->worldGrid.Load(this->cellTexture, 30, 30, 32,NULL);

	// Tilesets grid
	this->tilesets.tilesetTexture.loadFromFile("Assets/Sprites/Tilesets.png");

	sf::Vector2 tilesetsSize = this->tilesets.tilesetTexture.getSize();
	sf::Vector2i tilesetsGridSize = { static_cast<int>(tilesetsSize.x / 32), static_cast<int>(tilesetsSize.y / 32) };

	int startId = 1;
	this->tilesets.grid.Load(this->cellTexture, tilesetsGridSize.x, tilesetsGridSize.y, 32, &startId);

	// Load layers
	Layers::LayerTile* layerTile = new Layers::LayerTile();
	layerTile->name = "Layer 1";
	layerTile->pos = { 0, 0 };
	layerTile->grid.Load(this->cellTexture, 30, 30, 32);

	Layers layer;
	layer.layerType = Layers::LAYER_TYPE_TILES;
	layer.layer = layerTile;
	this->layers.push_back(layer);
}

void MapEditor::PollEvent(sf::RenderWindow& _renderWindow, sf::Event _event)
{

}

void MapEditor::Update(float const _dt, sf::RenderWindow& _renderWindow)
{
	// --- DockSpace principal ImGui ---
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(2);

	// --- Menu bar globale ---
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Fichier"))
		{
			if (ImGui::MenuItem("Nouveau")) { /* action */ }
			if (ImGui::MenuItem("Ouvrir")) { /* action */ }
			if (ImGui::MenuItem("Quitter")) { /* action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Édition"))
		{
			if (ImGui::MenuItem("Annuler")) { /* action */ }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// --- DockSpace ---
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Hierarchy");
	ImGui::Text("Encore une autre fenêtre dockable !");
	ImGui::End();

	ImGui::Begin("Inspector");
	ImGui::Text("Encore une autre fenêtre dockable !");
	ImGui::End();

	/* --- Scene Interface --- */
	ImGui::Begin("Scene");

	sf::Vector2f screenPos = ConvertImVec2toVector2f(ImGui::GetCursorScreenPos());
	ImVec2 avail = ImGui::GetContentRegionAvail();
	sf::Vector2f screenSize = ConvertImVec2toVector2f(avail);

	sf::Vector2f textureSize = static_cast<sf::Vector2f>(this->sceneInterface->getSize());

	if (textureSize != screenSize)
	{
		this->sceneInterface->create(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y));
		textureSize = screenSize;
	}

	sf::Vector2f mousePos = ConvertImVec2toVector2f(ImGui::GetMousePos());

	if (ScreenOverlap(screenPos, screenSize, mousePos))
	{
		sf::Vector2f localPos = mousePos - screenPos;

		sf::Vector2f worldPos = this->sceneInterface->mapPixelToCoords(static_cast<sf::Vector2i>(localPos));

		this->worldGrid.CellOverriding(worldPos);

		std::cout << "Mouse position in Scene Interface: " << localPos.x << ", " << localPos.y << std::endl;
	}

	this->sceneInterface->clear(sf::Color::Transparent);
	for (std::vector<Cell>& cellList : this->worldGrid.cellList)
	{
		for (Cell& cell : cellList)
		{
			this->sceneInterface->draw(cell.spr);
		}
	}

	ImGui::Image(
		reinterpret_cast<void*>(static_cast<intptr_t>(this->sceneInterface->getTexture().getNativeHandle())), avail,
		ImVec2(0, 1), ImVec2(1, 0)
	);
	ImGui::End();

	/* --- Tilesets Interface --- */
	ImGui::Begin("Tilesets");
	ImGui::Text("Encore une autre fenêtre dockable !");
	ImGui::Image(
		reinterpret_cast<void*>(static_cast<intptr_t>(this->tilesets.tilesetInterface->getTexture().getNativeHandle())),
		ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0)
	);

	ImGui::End();

	ImGui::End();


	//for (Interface& interface : this->interface)
	//{
	//	sf::FloatRect interfaceBounds = interface.sprite.getGlobalBounds();

	//	interface.active = false;

	//	if (interfaceBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
	//	{
	//		sf::Vector2f localSpr = interface.sprite.getInverseTransform().transformPoint(sf::Vector2f(mousePos));
	//		sf::Vector2i pixelTex = { static_cast<int>(localSpr.x), static_cast<int>(localSpr.y) };
	//		sf::Vector2f localPos = interface.rTexture->mapPixelToCoords(pixelTex);

	//		interface.active = true;

	//		if (interface.name == "Grid")
	//		{
	//			this->worldGrid.Draging(mousePos, TODO);
	//			this->worldGrid.CellOverriding(localPos);

	//			UpdateLayers(localPos);
	//		}
	//		else if (interface.name == "Inspector")
	//		{
	//			// Handle inspector logic here
	//		}
	//		else if (interface.name == "Layers")
	//		{

	//			// Handle layers logic here
	//		}
	//		else if (interface.name == "Tile sets")
	//		{
	//			localSpr = this->tilesets.tilesetInterface.sprite.getInverseTransform().transformPoint(sf::Vector2f(mousePos));
	//			pixelTex = { static_cast<int>(localSpr.x), static_cast<int>(localSpr.y) };
	//			localPos = this->tilesets.tilesetInterface.rTexture->mapPixelToCoords(pixelTex);

	//			this->tilesets.grid.Draging(mousePos, this->tilesets.tilesetInterface);
	//			this->tilesets.grid.CellOverriding(localPos);
	//		}
	//	}
	//}
}

void MapEditor::Draw(sf::RenderWindow& _renderWindow)
{
	
}

sf::Vector2f MapEditor::ConvertImVec2toVector2f(ImVec2 _v)
{
	return sf::Vector2f(_v.x, _v.y);
}

bool MapEditor::ScreenOverlap(sf::Vector2f _screenPos, sf::Vector2f _screenSize, sf::Vector2f _mousePos)
{
	return (_mousePos.x >= _screenPos.x && _mousePos.x <= _screenPos.x + _screenSize.x &&
		_mousePos.y >= _screenPos.y && _mousePos.y <= _screenPos.y + _screenSize.y);
}
