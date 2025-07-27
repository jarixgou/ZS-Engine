#include "ZS.h"

std::string GetComponentName(ZS::GameObject::Component::ComponentType type);
void DrawGameObjectNode(ZS::GameObject& _gO, ZS::GameObject*& _selectedGO);
void DrawGameObjectScene(ZS::GameObject& _gO, std::unique_ptr<sf::RenderTexture>& _render);

void CreateGrid(ZS::GameObject::Component::Grid& _grid, sf::Texture& _cellTexture);
void DestroyGrid(ZS::GameObject::Component::Grid& _grid);

sf::Vector2f ZS::ConverterImGui::ConvertImVec2toVector2f(const ImVec2& _vec)
{
	return sf::Vector2f(_vec.x, _vec.y);
}

ImVec2 ZS::ConverterImGui::ConvertVector2ftoImVec2(const sf::Vector2f& _vec)
{
	return ImVec2(_vec.x, _vec.y);
}

void ZS::Load()
{
	this->cellTexture.loadFromFile("Assets/Map editor/Sprites/cell.png");

	this->sceneRender = std::make_unique<sf::RenderTexture>();
	this->sceneRender->create(SCREEN_WIDTH, SCREEN_HEIGHT);

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

	/* --- Hierarchy --- */
	Hierarchy();

	/* --- Inspector --- */
	Inspector();

	/* --- Scene --- */
	Scene();

	/* --- Tileset --- */
	Tileset();

	ImGui::End(); // End DockSpace
}

void ZS::Draw(sf::RenderWindow& _renderWindow)
{

}

void ZS::Cleanup()
{

}

void ZS::Hierarchy()
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
					CreateGrid(*gridData, this->cellTexture);

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
					tilemapComponent.data = new GameObject::Component::Tilemap();
					tilemapGO->componentList.push_back(tilemapComponent);

					gridGO->child.push_back(tilemapGO);

					this->gameObjectList.push_back(gridGO);

					std::cout << "Created a new rectangular tilemap." << std::endl;

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

	int currentId = 0;
	for (GameObject*& gameObject : this->gameObjectList)
	{
		DrawGameObjectNode(*gameObject, this->selectedGO);
	}

	ImGui::End();
}

void ZS::Inspector()
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
					if (ImGui::DragInt("##GridSizeX", &grid->gridSize.x, 1, 0, INFINITY))
					{
						DestroyGrid(*grid);
						CreateGrid(*grid, this->cellTexture);
					}
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragInt("##GridSizeY", &grid->gridSize.y, 1, 0, INFINITY))
					{
						DestroyGrid(*grid);
						CreateGrid(*grid, this->cellTexture);
					}

					/* --- Cell size --- */
					ImGui::Text("Cell Size : ");
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragInt("##CellSizeX", &grid->cellSize.x, 1, INFINITY))
					{
						DestroyGrid(*grid);
						CreateGrid(*grid, this->cellTexture);
					}
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragInt("##CellSizeY", &grid->cellSize.y, 1, 0, INFINITY))
					{
						DestroyGrid(*grid);
						CreateGrid(*grid, this->cellTexture);
					}

					/* --- Cell gap --- */
					ImGui::Text("Cell Gap : ");
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragFloat("##CellGapX", &grid->cellGap.x, 0.1f, 0.0f, INFINITY))
					{
						DestroyGrid(*grid);
						CreateGrid(*grid, this->cellTexture);
					}
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (ImGui::DragFloat("##CellGapY", &grid->cellGap.y, 0.1f, 0.0f, INFINITY))
					{
						DestroyGrid(*grid);
						CreateGrid(*grid, this->cellTexture);
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

void ZS::Scene()
{
	ImGui::Begin("Scene");

	ImVec2 avail = ImGui::GetContentRegionAvail();
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(this->sceneRender->getSize());
	sf::Vector2f screenSize = ConverterImGui::ConvertImVec2toVector2f(avail);

	if (textureSize != screenSize)
	{
		this->sceneRender->create(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y));
		textureSize = screenSize;
	}

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

	ImGui::End();
}

void ZS::Tileset()
{
	ImGui::Begin("Tileset");
	if (ImGui::Button("Add Tileset"))
	{
		// Logic to add a new tileset
	}
	ImGui::Text("Tileset List:");
	ImGui::End();
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
		if (component.type == ZS::GameObject::Component::COMPONENT_TYPE_GRID)
		{
			ZS::GameObject::Component::Grid* grid = static_cast<ZS::GameObject::Component::Grid*>(component.data);

			for (const std::vector<ZS::GameObject::Component::Grid::Cell>& cellList : grid->cellList)
			{
				for (const ZS::GameObject::Component::Grid::Cell& cell : cellList)
				{
					_render->draw(cell.rect);
				}
			}
		}
	}

	bool hasChildren = !_gO.child.empty();

	if (hasChildren)
	{
		for (ZS::GameObject*& child : _gO.child)
		{
			DrawGameObjectScene(*child, _render);
		}
	}
}

void CreateGrid(ZS::GameObject::Component::Grid& _grid, sf::Texture& _cellTexture)
{
	for (int y = 0; y < _grid.gridSize.y; ++y)
	{
		std::vector<ZS::GameObject::Component::Grid::Cell> tempCellList;
		for (int x = 0; x < _grid.gridSize.x; ++x)
		{
			ZS::GameObject::Component::Grid::Cell tempCell;
			tempCell.id = y * _grid.gridSize.x + x;
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