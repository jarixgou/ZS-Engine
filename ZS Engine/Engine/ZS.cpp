#include "ZS.h"

void DrawGameObjectNode(ZS::GameObject& _gO, ZS::GameObject*& _selectedGO);
void DrawGameObjectScene(ZS::GameObject& _gO, std::unique_ptr<sf::RenderTexture>& _render);

sf::Vector2f ZS::ImGuiUtility::ConvertImVec2toVector2f(const ImVec2& _vec)
{
	return { _vec.x, _vec.y };
}

ImVec2 ZS::ImGuiUtility::ConvertVector2ftoImVec2(const sf::Vector2f& _vec)
{
	return { _vec.x, _vec.y };
}

void ZS::Load()
{
	cellTexture.loadFromFile("Assets/Map editor/Sprites/cell.png");

	sceneRender = std::make_unique<sf::RenderTexture>();
	sceneRender->create(SCREEN_WIDTH, SCREEN_HEIGHT);
	sceneView.setCenter(0, 0);

	tilesetRender = std::make_unique<sf::RenderTexture>();
	tilesetRender->create(SCREEN_WIDTH, SCREEN_HEIGHT);
	tilesetView.setCenter(0, 0);


	tileset.texture.loadFromFile("Assets/Sprites/Tilesets.png");
	tileset.sprite.setTexture(tileset.texture);

	tileset.grid.cellSize = { 32, 32 };
	sf::Vector2u textureSize = tileset.texture.getSize();
	tileset.grid.gridSize = { static_cast<int>(textureSize.x / tileset.grid.cellSize.x), static_cast<int>(textureSize.y / tileset.grid.cellSize.y) };

	tileset.grid.CreateGrid(cellTexture, true);

	/* --- Default tag --- */
	std::string playerTag = "Player";
	tagList.push_back(playerTag);

	std::string enemyTag = "Enemy";
	tagList.push_back(enemyTag);

	std::string npcTag = "NPC";
	tagList.push_back(npcTag);

	/* --- Default layer --- */
	Layer defaultLayer;
	defaultLayer.name = "Default";
	defaultLayer.id = 0;
	layerList.push_back(defaultLayer);
}

void ZS::PollEvent(sf::RenderWindow& _renderWindow, const sf::Event& _event)
{

}

void ZS::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	/* --- Paint tiles --- */
	if (selectedTileMapGO)
	{
		Component* tilemapComponent = selectedTileMapGO->GetComponent(Component::COMPONENT_TYPE_TILEMAP);

		if (tilemapComponent && selectedCell)
		{
			Tilemap* tilemapData = dynamic_cast<Tilemap*>(tilemapComponent->data.get());
			if (tilemapData)
			{
				tilemapData->PaintTiles(selectedCell, tileset.texture);
			}
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

					Component* gridComponent = new Component;
					gridComponent->type = Component::ComponentType::COMPONENT_TYPE_GRID;

					std::unique_ptr<Grid> gridData = std::make_unique<Grid>();
					gridData->CreateGrid(cellTexture, false);

					gridComponent->data = std::move(gridData);
					gridGO->componentList.push_back(gridComponent);

					GameObject* tilemapGO = new GameObject;
					tilemapGO->parent = gridGO;
					tilemapGO->name = "Tilemap";
					tilemapGO->tag = nullptr;
					tilemapGO->layer = nullptr;
					tilemapGO->transform = { };

					Component* tilemapComponent = new Component;
					tilemapComponent->type = Component::ComponentType::COMPONENT_TYPE_TILEMAP;
					std::unique_ptr<Tilemap> tilemapData = std::make_unique<Tilemap>();
					tilemapData->grid.CreateGrid(cellTexture, false);

					tilemapComponent->data = std::move(tilemapData);

					tilemapGO->componentList.push_back(tilemapComponent);

					tilemapList.push_back(tilemapGO);

					gridGO->child.push_back(tilemapGO);

					gameObjectList.push_back(gridGO);
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

	for (GameObject*& gameObject : gameObjectList)
	{
		DrawGameObjectNode(*gameObject, selectedGO);
	}

	ImGui::End();
}

void ZS::InspectorInterface()
{
	ImGui::Begin("Inspector");

	if (selectedGO)
	{
		ImGui::Text("Name : ");
		ImGui::SameLine();
		char buffer[256];
		strcpy_s(buffer, sizeof(buffer), selectedGO->name.c_str());
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
		{
			selectedGO->name = std::string(buffer);
		}

		if (ImGui::CollapsingHeader("Transform"))
		{
			/* --- Position --- */
			ImGui::Text("Position : ");
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##PositionX", &selectedGO->transform.pos.x, 0.5f, -INFINITY, INFINITY);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##PositionY", &selectedGO->transform.pos.y, 0.5f, -INFINITY, INFINITY);

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
			ImGui::DragFloat("##ScaleX", &selectedGO->transform.scale.x, 0.01f, 0.01f, 10.0f);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat("##ScaleY", &selectedGO->transform.scale.y, 0.01f, 0.01f, 10.0f);
		}

		for (Component* component : selectedGO->componentList)
		{
			std::string componentName = component->GetComponentName();

			if (ImGui::CollapsingHeader(componentName.c_str()))
			{
				if (component->type == Component::COMPONENT_TYPE_GRID)
				{
					Grid* grid = dynamic_cast<Grid*>(component->data.get());
					if (grid)
					{
						/* --- Gride size --- */
						ImGui::Text("Grid Size : ");
						ImGui::SameLine();
						ImGui::Text("X");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(100);

						static int prevGridSizeX = -1;
						ImGui::DragInt("##GridSizeX", &grid->gridSize.x, 1, 0, INT_MAX);
						if (ImGui::IsItemDeactivatedAfterEdit() && prevGridSizeX != grid->gridSize.x)
						{
							selectedGO->ResizeGrid(*grid, cellTexture, tileset.texture);

							prevGridSizeX = grid->gridSize.x;
						}

						ImGui::SameLine();
						ImGui::Text("Y");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(100);

						static int prevGridSizeY = -1;
						ImGui::DragInt("##GridSizeY", &grid->gridSize.y, 1, 0, INT_MAX);
						if (ImGui::IsItemDeactivatedAfterEdit() && grid->gridSize.y != prevGridSizeY)
						{
							selectedGO->ResizeGrid(*grid, cellTexture, tileset.texture);
						}

						/* --- Cell size --- */
						ImGui::Text("Cell Size : ");
						ImGui::SameLine();
						ImGui::Text("X");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(100);
						if (ImGui::DragInt("##CellSizeX", &grid->cellSize.x, 1, INT_MAX))
						{
							selectedGO->ResizeGrid(*grid, cellTexture, tileset.texture);
						}
						ImGui::SameLine();
						ImGui::Text("Y");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(100);
						if (ImGui::DragInt("##CellSizeY", &grid->cellSize.y, 1, 0, INT_MAX))
						{
							selectedGO->ResizeGrid(*grid, cellTexture, tileset.texture);
						}

						/* --- Cell gap --- */
						ImGui::Text("Cell Gap : ");
						ImGui::SameLine();
						ImGui::Text("X");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(100);
						if (ImGui::DragFloat("##CellGapX", &grid->cellGap.x, 0.1f, 0.0f, INFINITY))
						{
							selectedGO->ResizeGrid(*grid, cellTexture, tileset.texture);
						}
						ImGui::SameLine();
						ImGui::Text("Y");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(100);
						if (ImGui::DragFloat("##CellGapY", &grid->cellGap.y, 0.1f, 0.0f, INFINITY))
						{
							selectedGO->ResizeGrid(*grid, cellTexture, tileset.texture);
						}
					}
				}

				if (component->type == Component::COMPONENT_TYPE_TILEMAP)
				{
					Tilemap* tilemap = dynamic_cast<Tilemap*>(component->data.get());

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
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(sceneRender->getSize());
	sf::Vector2f screenSize = ImGuiUtility::ConvertImVec2toVector2f(avail);

	sf::Vector2f mousePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetMousePos());

	if (screenSize.x > 1 && screenSize.y > 1) // sécurité minimale
	{
		if (textureSize != screenSize)
		{
			sceneRender->create(static_cast<unsigned int>(screenSize.x),
				static_cast<unsigned int>(screenSize.y));
			sceneView.setSize(screenSize);
			textureSize = screenSize;
		}
	}

	sceneRender->setView(sceneView);

	sceneRender->clear(sf::Color::Transparent);

	for (GameObject*& gameObject : gameObjectList)
	{
		DrawGameObjectScene(*gameObject, sceneRender);
	}

	sceneRender->display();

	ImGui::Image(
		reinterpret_cast<void*>(static_cast<intptr_t>(sceneRender->getTexture().getNativeHandle())), avail,
		ImVec2(0, 1), ImVec2(1, 0)
	);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
	{
		float scroolWheel = ImGui::GetIO().MouseWheel;
		if (scroolWheel > 0)
		{
			sceneView.zoom(0.9f);
		}
		else if (scroolWheel < 0)
		{
			sceneView.zoom(1.1f);
		}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			sceneView.move(-delta.x, -delta.y);
		}

		sf::Vector2f imagePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetItemRectMin());

		sf::Vector2f localPos = mousePos - imagePos;
		sf::Vector2f worldPos = sceneRender->mapPixelToCoords(static_cast<sf::Vector2i>(localPos));

		for (auto& gameObject : gameObjectList)
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
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(tilesetRender->getSize());
	sf::Vector2f screenSize = ImGuiUtility::ConvertImVec2toVector2f(avail);

	sf::Vector2f mousePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetMousePos());

	if (screenSize.x > 1 && screenSize.y > 1)
	{
		if (textureSize != screenSize)
		{
			tilesetRender->create(static_cast<unsigned int>(screenSize.x),
				static_cast<unsigned int>(screenSize.y));
			tilesetView.setSize(screenSize);
			textureSize = screenSize;
		}
	}

	std::string tilemapName;

	selectedTileMapGO ? tilemapName = selectedTileMapGO->name : tilemapName = "None";

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

		for (GameObject* tilemapList : tilemapList)
		{
			const bool isSelected = (tilemapList == selectedTileMapGO);
			if (filter.PassFilter(tilemapList->name.c_str()))
			{
				if (ImGui::Selectable(tilemapList->name.c_str(), isSelected))
				{
					selectedTileMapGO = tilemapList;
				}
			}
		}

		ImGui::EndCombo();
	}

	tilesetRender->setView(tilesetView);

	tilesetRender->clear(sf::Color::Transparent);

	tilesetRender->draw(tileset.sprite);

	for (auto& cellList : tileset.grid.cellList)
	{
		for (auto& cell : cellList)
		{
			tilesetRender->draw(cell.rect);
		}
	}

	tilesetRender->display();

	ImGui::Image(
		reinterpret_cast<void*>(static_cast<intptr_t>(tilesetRender->getTexture().getNativeHandle())), avail,
		ImVec2(0, 1), ImVec2(1, 0)
	);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
	{
		sf::Vector2f imagePos = ImGuiUtility::ConvertImVec2toVector2f(ImGui::GetItemRectMin());

		float scroolWheel = ImGui::GetIO().MouseWheel;
		if (scroolWheel > 0)
		{
			tilesetView.zoom(0.9f);
		}
		else if (scroolWheel < 0)
		{
			tilesetView.zoom(1.1f);
		}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			tilesetView.move(-delta.x, -delta.y);
		}

		sf::Vector2f localPos = mousePos - imagePos;
		sf::Vector2f worldPos = tilesetRender->mapPixelToCoords(static_cast<sf::Vector2i>(localPos));

		tileset.grid.CellOverriding(worldPos);
		tileset.grid.SelectCell(&selectedCell);
	}

	ImGui::End();
}

void ZS::UpdateGrid(GameObject& _gO, sf::Vector2f _worldPos)
{
	for (auto component : _gO.componentList)
	{
		if (component->type == Component::COMPONENT_TYPE_GRID)
		{
			Grid* grid = dynamic_cast<Grid*>(component->data.get());
			if (grid)
			{
				grid->CellOverriding(_worldPos);
			}
		}
		else if (component->type == Component::COMPONENT_TYPE_TILEMAP)
		{
			Tilemap* tilemap = dynamic_cast<Tilemap*>(component->data.get());
			if (tilemap)
			{
				tilemap->grid.CellOverriding(_worldPos);
			}
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
	for (auto child : _gO.child)
	{
		DrawGameObjectScene(*child, _render);
	}

	for (auto component : _gO.componentList)
	{
		switch (component->type)
		{
		case ZS::Component::COMPONENT_TYPE_NONE:
			break;
		case ZS::Component::COMPONENT_TYPE_GRID:
		{
			ZS::Grid& grid = dynamic_cast<ZS::Grid&>(*component->data);

			for (const auto& cellList : grid.cellList)
			{
				for (const auto& cell : cellList)
				{
					_render->draw(cell.rect);
				}
			}
			break;
		}
		case ZS::Component::COMPONENT_TYPE_TILEMAP:
		{
			ZS::Tilemap& tilemap = dynamic_cast<ZS::Tilemap&>(*component->data);

			for (auto& cellList : tilemap.grid.cellList)
			{
				for (auto& cell : cellList)
				{
					_render->draw(cell.spr);
				}
			}
			break;
		}
		case ZS::Component::COMPONENT_TYPE_SPRITE:
			break;
		case ZS::Component::COMPONENT_TYPE_ANIMATION:
			break;
		case ZS::Component::COMPONENT_TYPE_TEXT:
			break;
		case ZS::Component::COMPONENT_TYPE_COLLIDER:
			break;
		case ZS::Component::COMPONENT_TYPE_RIGIDBODY:
			break;
		default:
			break;
		}
	}
}