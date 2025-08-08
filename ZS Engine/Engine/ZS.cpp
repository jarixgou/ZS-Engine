#include "ZS.h"

std::string GetComponentName(ZS::GameObject::Component::ComponentType type);
void DrawGameObjectNode(ZS::GameObject& _gO, ZS::GameObject*& _selectedGO);
void DrawGameObjectScene(ZS::GameObject& _gO, std::unique_ptr<sf::RenderTexture>& _render);

void CreateGrid(ZS::GameObject::Component::Grid& _grid, sf::Texture& _cellTexture, bool _giveID);
void DestroyGrid(ZS::GameObject::Component::Grid& _grid);

sf::Vector2f ZS::ImGuiUtility::ConvertImVec2toVector2f(const ImVec2& _vec)
{
	return { _vec.x, _vec.y };
}

ImVec2 ZS::ImGuiUtility::ConvertVector2ftoImVec2(const sf::Vector2f& _vec)
{
	return { _vec.x, _vec.y };
}

void ZS::GameObject::Component::Grid::CellOverriding(sf::Vector2f _worldPos)
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

void ZS::GameObject::Component::Grid::SelectCell(Cell** _selectedCell)
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

void ZS::GameObject::Component::Tilemap::PaintTiles(Grid::Cell* _selectedCell, sf::Texture& _tilesetTexture)
{
	for (auto& cellList : this->grid.cellList)
	{
		for (auto& cell : cellList)
		{
			if (cell.mouseInside && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _selectedCell)
			{
				cell.spr.setTexture(_tilesetTexture);

				sf::IntRect textureRect = sf::IntRect(_selectedCell->column * this->grid.cellSize.x, _selectedCell->line * this->grid.cellSize.y,
					this->grid.cellSize.x, this->grid.cellSize.y);

				cell.spr.setTextureRect(textureRect);
				cell.spr.setPosition(
					cell.column * (this->grid.cellSize.x + this->grid.cellGap.x),
					cell.line * (this->grid.cellSize.y + this->grid.cellGap.y)
				);

				cell.id = _selectedCell->id;
			}
		}
	}
}

void ZS::Load()
{
	this->cellTexture.loadFromFile("Assets/Map editor/Sprites/cell.png");

	this->sceneRender = std::make_unique<sf::RenderTexture>();
	this->sceneRender->create(SCREEN_WIDTH, SCREEN_HEIGHT);
	this->sceneView.setCenter(0, 0);

	this->tilesetRender = std::make_unique<sf::RenderTexture>();
	this->tilesetRender->create(SCREEN_WIDTH, SCREEN_HEIGHT);
	this->tilesetView.setCenter(0, 0);


	this->tileset.texture.loadFromFile("Assets/Sprites/Tilesets.png");
	this->tileset.sprite.setTexture(this->tileset.texture);

	this->tileset.grid.cellSize = { 32, 32 };
	sf::Vector2u textureSize = this->tileset.texture.getSize();
	this->tileset.grid.gridSize = { static_cast<int>(textureSize.x / this->tileset.grid.cellSize.x), static_cast<int>(textureSize.y / this->tileset.grid.cellSize.y) };

	CreateGrid(this->tileset.grid, this->cellTexture, true);

	/* --- Default tag --- */
	std::string playerTag = "Player";
	this->tagList.push_back(playerTag);

	std::string enemyTag = "Enemy";
	this->tagList.push_back(enemyTag);

	std::string npcTag = "NPC";
	this->tagList.push_back(npcTag);

	/* --- Default layer --- */
	Layer defaultLayer;
	defaultLayer.name = "Default";
	defaultLayer.id = 0;
	this->layerList.push_back(defaultLayer);
}

void ZS::PollEvent(sf::RenderWindow& _renderWindow, const sf::Event& _event)
{

}

void ZS::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	/* --- Paint tiles --- */
	if (this->selectedTileMapGO)
	{
		GameObject::Component* tilemapComponent = GetComponent(this->selectedTileMapGO, GameObject::Component::COMPONENT_TYPE_TILEMAP);

		if (tilemapComponent && this->selectedCell)
		{
			GameObject::Component::Tilemap* tilemapData = static_cast<GameObject::Component::Tilemap*>(tilemapComponent->data);
			tilemapData->PaintTiles(this->selectedCell, this->tileset.texture);
		}
	}

	/* --- Dock space --- */
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("DockSpace", nullptr, windowFlags);
	ImGui::PopStyleVar(2);

	/* --- Menu bar --- */
	if (ImGui::BeginMenuBar())
	{
		// File menu
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{

			}

			if (ImGui::MenuItem("Open"))
			{

			}

			if (ImGui::MenuItem("Quit"))
			{

			}

			ImGui::EndMenu();
		}

		// Edit menu
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo")) { /* action */ }

			if (ImGui::MenuItem("Redo")) { /* action */ }

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_PassthruCentralNode);

	/* --- HierarchyInterface --- */
	HierarchyInterface();

	/* --- InspectorInterface --- */
	InspectorInterface();

	/* --- SceneInterface --- */
	SceneInterface();

	/* --- TilesetInterface --- */
	TilesetInterface();

	ImGui::End(); // End DockSpace
}

void ZS::Draw(sf::RenderWindow& _renderWindow)
{

}

void ZS::Cleanup()
{

}

void ZS::HierarchyInterface()
{
	ImGui::Begin("Hierarchy");

	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("Create menu");
	}

	if (ImGui::Button("+"))
	{
		ImGui::OpenPopup("Create menu");
	}

	if (ImGui::BeginPopup("Create menu"))
	{
		if (ImGui::MenuItem("Create empty GameObject", "Ctrl+Shift+N"))
		{

		}

		if (ImGui::BeginMenu("2D Object"))
		{
			if (ImGui::BeginMenu("Shape"))
			{
				if (ImGui::MenuItem("Sprite"))
				{

				}

				if (ImGui::MenuItem("Circle"))
				{

				}

				if (ImGui::MenuItem("Rectangle"))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tilemap"))
			{
				if (ImGui::MenuItem("Rectangular"))
				{
					GameObject* gridGO = new GameObject;
					gridGO->parent = nullptr;
					gridGO->name = "Grid";
					gridGO->tag = nullptr;
					gridGO->layer = nullptr;
					gridGO->transform = { };

					GameObject::Component gridComponent;
					gridComponent.type = GameObject::Component::ComponentType::COMPONENT_TYPE_GRID;

					GameObject::Component::Grid* gridData = new GameObject::Component::Grid;
					CreateGrid(*gridData, this->cellTexture, false);

					gridComponent.data = gridData;

					gridGO->componentList.push_back(gridComponent);

					GameObject* tilemapGO = new GameObject;
					tilemapGO->parent = gridGO;
					tilemapGO->name = "Tilemap";
					tilemapGO->tag = nullptr;
					tilemapGO->layer = nullptr;
					tilemapGO->transform = { };

					GameObject::Component tilemapComponent;
					tilemapComponent.type = GameObject::Component::ComponentType::COMPONENT_TYPE_TILEMAP;
					GameObject::Component::Tilemap* tilemapData = new GameObject::Component::Tilemap;
					CreateGrid(tilemapData->grid, this->cellTexture, false);

					tilemapComponent.data = tilemapData;

					tilemapGO->componentList.push_back(tilemapComponent);

					this->tilemapList.push_back(tilemapGO);

					gridGO->child.push_back(tilemapGO);

					this->gameObjectList.push_back(gridGO);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("UI"))
		{
			if (ImGui::MenuItem("Image"))
			{

			}

			if (ImGui::MenuItem("Text"))
			{

			}

			if (ImGui::MenuItem("Button"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Light"))
		{
			if (ImGui::MenuItem("Point light"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	static int selected = -1;

	for (GameObject*& gameObject : this->gameObjectList)
	{
		DrawGameObjectNode(*gameObject, this->selectedGO);
	}

	ImGui::End();
}

void ZS::InspectorInterface()
{
	ImGui::Begin("Inspector");

	if (this->selectedGO)
	{
		ImGui::Text("Name : ");
		ImGui::SameLine();
		char buffer[256];
		strcpy_s(buffer, sizeof(buffer), selectedGO->name.c_str());
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
		{
			this->selectedGO->name = std::string(buffer);
		}

		if (ImGui::CollapsingHeader("Transform"))
		{
			/* --- Position --- */
			ImGui::Text("Position : ");
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##PositionX", &this->selectedGO->transform.pos.x, 0.5f, -INFINITY, INFINITY);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##PositionY", &this->selectedGO->transform.pos.y, 0.5f, -INFINITY, INFINITY);

			float rot = selectedGO->transform.rotation;
			ImGui::Text("Rotation : ");
			ImGui::SameLine();
			ImGui::DragFloat("##Rotation", &rot, 1, -360.0f, 360.0f);
			selectedGO->transform.rotation = rot;

			ImGui::Text("Scale : ");
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##ScaleX", &this->selectedGO->transform.scale.x, 0.01f, 0.01f, 10.0f);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##ScaleY", &this->selectedGO->transform.scale.y, 0.01f, 0.01f, 10.0f);
		}

		for (GameObject::Component& component : this->selectedGO->componentList)
		{
			std::string componentName = GetComponentName(component.type);

			if (ImGui::CollapsingHeader(componentName.c_str()))
			{
				if (component.type == GameObject::Component::COMPONENT_TYPE_GRID)
				{
					GameObject::Component::Grid* grid = static_cast<GameObject::Component::Grid*>(component.data);

					/* --- Gride size --- */
					ImGui::Text("Grid Size : ");
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);

					static int prevGridSizeX = -1;
					ImGui::DragInt("##GridSizeX", &grid->gridSize.x, 1, 0, INT_MAX);
					if (ImGui::IsItemDeactivatedAfterEdit()) {
						if (prevGridSizeX != grid->gridSize.x) {
							ResizeGrid(*this->selectedGO, grid->gridSize, grid->cellSize, grid->cellGap);
							prevGridSizeX = grid->gridSize.x;
						}
					}

					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
								
					static int prevGridSizeY = -1;
					ImGui::DragInt("##GridSizeY", &grid->gridSize.y, 1, 0, INT_MAX);
					if (ImGui::IsItemDeactivatedAfterEdit() && grid->gridSize.y != prevGridSizeY) {
					    ResizeGrid(*this->selectedGO, grid->gridSize, grid->cellSize, grid->cellGap);
					}

					/* --- Cell size --- */
					ImGui::Text("Cell Size : ");
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragInt("##CellSizeX", &grid->cellSize.x, 1, INT_MAX))
					{
						ResizeGrid(*this->selectedGO, grid->gridSize, grid->cellSize, grid->cellGap);
					}
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragInt("##CellSizeY", &grid->cellSize.y, 1, 0, INT_MAX))
					{
						ResizeGrid(*this->selectedGO, grid->gridSize, grid->cellSize, grid->cellGap);
					}

					/* --- Cell gap --- */
					ImGui::Text("Cell Gap : ");
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragFloat("##CellGapX", &grid->cellGap.x, 0.1f, 0.0f, INFINITY))
					{
						ResizeGrid(*this->selectedGO, grid->gridSize, grid->cellSize, grid->cellGap);
					}
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragFloat("##CellGapY", &grid->cellGap.y, 0.1f, 0.0f, INFINITY))
					{
						ResizeGrid(*this->selectedGO, grid->gridSize, grid->cellSize, grid->cellGap);
					}
				}

				if (component.type == GameObject::Component::COMPONENT_TYPE_TILEMAP)
				{
					GameObject::Component::Tilemap* tilemap = static_cast<GameObject::Component::Tilemap*>(component.data);

					/* --- Sprite color --- */
					float color[4] = { tilemap->color.r / 255.f, tilemap->color.g / 255.f, tilemap->color.b / 255.f, tilemap->color.a / 255.f };

					ImGui::Text("Sprite Color : ");
					ImGui::SameLine();
					if (ImGui::ColorEdit4("Sprite Color", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
					{
						tilemap->color.r = static_cast<sf::Uint8>(color[0] * 255.f);
						tilemap->color.g = static_cast<sf::Uint8>(color[1] * 255.f);
						tilemap->color.b = static_cast<sf::Uint8>(color[2] * 255.f);
						tilemap->color.a = static_cast<sf::Uint8>(color[3] * 255.f);
					}
				}
			}
		}

		if (ImGui::Button("Add component"))
		{

		}
	}
	else
	{
		ImGui::Text("Please select a Game Object.");
	}

	ImGui::End();
}

void ZS::SceneInterface()
{
	ImGui::Begin("Scene");

	ImVec2 avail = ImGui::GetContentRegionAvail();
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(this->sceneRender->getSize());
	sf::Vector2f screenSize = ImGuiUtility::ConvertImVec2toVector2f(avail);

	sf::Vector2f mousePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetMousePos());

	if (screenSize.x > 1 && screenSize.y > 1) // sécurité minimale
	{
		if (textureSize != screenSize)
		{
			this->sceneRender->create(static_cast<unsigned int>(screenSize.x),
				static_cast<unsigned int>(screenSize.y));
			this->sceneView.setSize(screenSize);
			textureSize = screenSize;
		}
	}

	this->sceneRender->setView(this->sceneView);

	this->sceneRender->clear(sf::Color::Transparent);

	for (GameObject*& gameObject : this->gameObjectList)
	{
		DrawGameObjectScene(*gameObject, this->sceneRender);
	}

	this->sceneRender->display();

	ImGui::Image(
		reinterpret_cast<void*>(static_cast<intptr_t>(this->sceneRender->getTexture().getNativeHandle())), avail,
		ImVec2(0, 1), ImVec2(1, 0)
	);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
	{
		float scroolWheel = ImGui::GetIO().MouseWheel;
		if (scroolWheel > 0)
		{
			this->sceneView.zoom(0.9f);
		}
		else if (scroolWheel < 0)
		{
			this->sceneView.zoom(1.1f);
		}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			this->sceneView.move(-delta.x, -delta.y);
		}

		sf::Vector2f imagePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetItemRectMin());

		sf::Vector2f localPos = mousePos - imagePos;
		sf::Vector2f worldPos = this->sceneRender->mapPixelToCoords(static_cast<sf::Vector2i>(localPos));

		for (auto& gameObject : this->gameObjectList)
		{
			UpdateGrid(*gameObject, worldPos);
		}
	}

	ImGui::End();
}

void ZS::TilesetInterface()
{
	ImGui::Begin("Tileset");

	ImVec2 avail = ImGui::GetContentRegionAvail();
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(this->tilesetRender->getSize());
	sf::Vector2f screenSize = ImGuiUtility::ConvertImVec2toVector2f(avail);

	sf::Vector2f mousePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetMousePos());

	if (screenSize.x > 1 && screenSize.y > 1)
	{
		if (textureSize != screenSize)
		{
			this->tilesetRender->create(static_cast<unsigned int>(screenSize.x),
				static_cast<unsigned int>(screenSize.y));
			this->tilesetView.setSize(screenSize);
			textureSize = screenSize;
		}
	}

	std::string tilemapName;

	this->selectedTileMapGO ? tilemapName = this->selectedTileMapGO->name : tilemapName = "None";

	ImGui::Text("Tile Map active : ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150);
	if (ImGui::BeginCombo("##Combo", tilemapName.c_str()))
	{
		static ImGuiTextFilter filter;
		if (ImGui::IsWindowAppearing())
		{
			ImGui::SetKeyboardFocusHere();
			filter.Clear();
		}
		filter.Draw("##Filter", -FLT_MIN);

		for (GameObject* tilemapList : this->tilemapList)
		{
			const bool isSelected = (tilemapList == this->selectedTileMapGO);
			if (filter.PassFilter(tilemapList->name.c_str()))
			{
				if (ImGui::Selectable(tilemapList->name.c_str(), isSelected))
				{
					this->selectedTileMapGO = tilemapList;
				}
			}
		}

		ImGui::EndCombo();
	}

	this->tilesetRender->setView(this->tilesetView);

	this->tilesetRender->clear(sf::Color::Transparent);

	this->tilesetRender->draw(this->tileset.sprite);

	for (auto& cellList : this->tileset.grid.cellList)
	{
		for (auto& cell : cellList)
		{
			this->tilesetRender->draw(cell.rect);
		}
	}

	this->tilesetRender->display();

	ImGui::Image(
		reinterpret_cast<void*>(static_cast<intptr_t>(this->tilesetRender->getTexture().getNativeHandle())), avail,
		ImVec2(0, 1), ImVec2(1, 0)
	);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
	{
		sf::Vector2f imagePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetItemRectMin());

		float scroolWheel = ImGui::GetIO().MouseWheel;
		if (scroolWheel > 0)
		{
			this->tilesetView.zoom(0.9f);
		}
		else if (scroolWheel < 0)
		{
			this->tilesetView.zoom(1.1f);
		}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			this->tilesetView.move(-delta.x, -delta.y);
		}

		sf::Vector2f localPos = mousePos - imagePos;
		sf::Vector2f worldPos = this->tilesetRender->mapPixelToCoords(static_cast<sf::Vector2i>(localPos));

		this->tileset.grid.CellOverriding(worldPos);
		this->tileset.grid.SelectCell(&this->selectedCell);
	}

	ImGui::End();
}

ZS::GameObject::Component* ZS::GetComponent(GameObject* const _gO, GameObject::Component::ComponentType _type)
{
	for (GameObject::Component& component : _gO->componentList)
	{
		if (component.type == _type)
		{
			return &component;
		}
	}

	for (auto child : _gO->child)
	{
		GameObject::Component* componentFound = GetComponent(child, _type);

		if (componentFound)
		{
			return componentFound;
		}
	}

	return nullptr;
}

void ZS::UpdateGrid(GameObject& _gO, sf::Vector2f _worldPos)
{
	for (auto& component : _gO.componentList)
	{
		if (component.type == GameObject::Component::COMPONENT_TYPE_GRID)
		{
			GameObject::Component::Grid* grid = static_cast<GameObject::Component::Grid*>(component.data);
			grid->CellOverriding(_worldPos);
		}
		else if (component.type == GameObject::Component::COMPONENT_TYPE_TILEMAP)
		{
			GameObject::Component::Tilemap* tilemap = static_cast<GameObject::Component::Tilemap*>(component.data);
			tilemap->grid.CellOverriding(_worldPos);
		}
	}

	if (!_gO.child.empty())
	{
		for (auto& child : _gO.child)
		{
			UpdateGrid(*child, _worldPos);
		}
	}
}

void ZS::ResizeGrid(GameObject& _gO, sf::Vector2i& _gridSize, sf::Vector2i& _cellSize, sf::Vector2f& _cellGap)
{
	for (auto& component : _gO.componentList)
	{
		if (component.type == GameObject::Component::COMPONENT_TYPE_GRID)
		{
			GameObject::Component::Grid* grid = static_cast<GameObject::Component::Grid*>(component.data);

			DestroyGrid(*grid);
			CreateGrid(*grid, this->cellTexture, false);
		}
		else if (component.type == GameObject::Component::COMPONENT_TYPE_TILEMAP)
		{
			GameObject::Component::Tilemap* tilemap = static_cast<GameObject::Component::Tilemap*>(component.data);

			tilemap->grid.gridSize = _gridSize;
			tilemap->grid.cellSize = _cellSize;
			tilemap->grid.cellGap = _cellGap;

			auto gridID = SaveGridID(tilemap->grid);
			DestroyGrid(tilemap->grid);
			CreateGrid(tilemap->grid, this->cellTexture, true);
			RestoreGridID(tilemap->grid, gridID);
		}
	}

	if (!_gO.child.empty())
	{
		for (auto& child : _gO.child)
		{
			ResizeGrid(*child, _gridSize, _cellSize, _cellGap);
		}
	}
}

std::vector<std::vector<ZS::GameObject::Component::Grid::Cell>> ZS::SaveGridID(GameObject::Component::Grid& _grid)
{
	return _grid.cellList;
}

void ZS::RestoreGridID(GameObject::Component::Grid& _grid, const std::vector<std::vector<GameObject::Component::Grid::Cell>>& _savedGridID)
{
	for (auto& cellList : _grid.cellList)
	{
		for (auto& cell : cellList)
		{
			if (cell.line < _savedGridID.size() && cell.column < _savedGridID[cell.line].size())
			{
				const auto& savedCell = _savedGridID[cell.line][cell.column];
				cell.spr = savedCell.spr;
				cell.id = savedCell.id;
			}
		}
	}
}

std::string GetComponentName(ZS::GameObject::Component::ComponentType type)
{
	switch (type)
	{
	case ZS::GameObject::Component::COMPONENT_TYPE_NONE:
		return "None";
	case ZS::GameObject::Component::COMPONENT_TYPE_GRID:
		return "Grid";
	case ZS::GameObject::Component::COMPONENT_TYPE_TILEMAP:
		return "Tilemap";
	case ZS::GameObject::Component::COMPONENT_TYPE_SPRITE:
		return "Sprite";
	case ZS::GameObject::Component::COMPONENT_TYPE_ANIMATION:
		return "Animation";
	case ZS::GameObject::Component::COMPONENT_TYPE_TEXT:
		return "Text";
	case ZS::GameObject::Component::COMPONENT_TYPE_COLLIDER:
		return "Collider";
	case ZS::GameObject::Component::COMPONENT_TYPE_RIGIDBODY:
		return "Rigid Body";
	default:
		return "Unknown";
	}
}

void DrawGameObjectNode(ZS::GameObject& _gO, ZS::GameObject*& _selectedGO)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (&_gO == _selectedGO)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool hasChildren = !_gO.child.empty();

	if (!hasChildren)
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool open = ImGui::TreeNodeEx(&_gO, flags, "%s", _gO.name.c_str());

	if (ImGui::IsItemClicked())
	{
		_selectedGO = &_gO;
	}

	if (open && hasChildren)
	{
		for (ZS::GameObject*& child : _gO.child)
		{
			DrawGameObjectNode(*child, _selectedGO);
		}
		ImGui::TreePop();
	}
}

void DrawGameObjectScene(ZS::GameObject& _gO, std::unique_ptr<sf::RenderTexture>& _render)
{
	for (ZS::GameObject::Component& component : _gO.componentList)
	{
		switch (component.type)
		{
		case ZS::GameObject::Component::COMPONENT_TYPE_NONE:
			break;
		case ZS::GameObject::Component::COMPONENT_TYPE_GRID:
		{
			ZS::GameObject::Component::Grid* grid = static_cast<ZS::GameObject::Component::Grid*>(component.data);

			for (const auto& cellList : grid->cellList)
			{
				for (const auto& cell : cellList)
				{
					_render->draw(cell.rect);
				}
			}
			break;
		}
		case ZS::GameObject::Component::COMPONENT_TYPE_TILEMAP:
		{
			ZS::GameObject::Component::Tilemap* tilemap = static_cast<ZS::GameObject::Component::Tilemap*>(component.data);

			for (auto& cellList : tilemap->grid.cellList)
			{
				for (auto& cell : cellList)
				{
					_render->draw(cell.spr);
				}
			}
			break;
		}
		case ZS::GameObject::Component::COMPONENT_TYPE_SPRITE:
			break;
		case ZS::GameObject::Component::COMPONENT_TYPE_ANIMATION:
			break;
		case ZS::GameObject::Component::COMPONENT_TYPE_TEXT:
			break;
		case ZS::GameObject::Component::COMPONENT_TYPE_COLLIDER:
			break;
		case ZS::GameObject::Component::COMPONENT_TYPE_RIGIDBODY:
			break;
		default:
			break;
		}
	}

	if (!_gO.child.empty())
	{
		for (ZS::GameObject*& child : _gO.child)
		{
			DrawGameObjectScene(*child, _render);
		}
	}
}

void CreateGrid(ZS::GameObject::Component::Grid& _grid, sf::Texture& _cellTexture, bool _giveID)
{
	size_t id = 0;

	for (int y = 0; y < _grid.gridSize.y; ++y)
	{
		std::vector<ZS::GameObject::Component::Grid::Cell> tempCellList;
		for (int x = 0; x < _grid.gridSize.x; ++x)
		{
			ZS::GameObject::Component::Grid::Cell tempCell;

			_giveID ? tempCell.id = ++id : tempCell.id = 0;

			tempCell.line = y;
			tempCell.column = x;

			tempCell.mouseInside = false;
			tempCell.selected = false;

			tempCell.rect.setSize(sf::Vector2f(_grid.cellSize.x, _grid.cellSize.y));
			tempCell.rect.setPosition(static_cast<float>(x * (_grid.cellSize.x + _grid.cellGap.x)), static_cast<float>(y * (_grid.cellSize.y + _grid.cellGap.y)));
			tempCell.rect.setTexture(&_cellTexture);

			tempCellList.push_back(tempCell);
		}
		_grid.cellList.push_back(tempCellList);
	}
}

void DestroyGrid(ZS::GameObject::Component::Grid& _grid)
{
	for (auto& cellList : _grid.cellList)
	{
		cellList.clear();
	}

	_grid.cellList.clear();
}