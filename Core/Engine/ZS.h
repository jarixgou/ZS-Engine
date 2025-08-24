#pragma once

#include <string>
#include <functional>
#include <filesystem>

#include "../Common.h"
#include "Layer/Layer.h"
#include "GameObject/GameObject.h"
#include "GameObject/Component/Component.h"
#include "GameObject/Component/Grid/Grid.h"
#include "GameObject/Component/Tilemap/Tilemap.h"

#define ZS_VERSION "0.0.1"

namespace ZS
{
	typedef class ImGuiUtility
	{
	public:
		static sf::Vector2f ConvertImVec2toVector2f(const ImVec2& _vec);
		static ImVec2 ConvertVector2ftoImVec2(const sf::Vector2f& _vec);
	}ImGuiUtility;


	typedef class Tileset
	{
	public:
		Grid grid;

		sf::Texture texture;
		sf::Sprite sprite;
	}Tileset;

	void Load();
	void PollEvent(sf::RenderWindow& _renderWindow, const sf::Event& _event);
	void Update(sf::RenderWindow& _renderWindow, float _dt);
	void Draw(sf::RenderWindow& _renderWindow);
	void Cleanup();

	inline sf::View sceneView;
	inline std::unique_ptr<sf::RenderTexture> sceneRender;

	inline sf::View tilesetView;
	inline std::unique_ptr<sf::RenderTexture> tilesetRender;
	inline Tileset tileset;

	inline std::vector<std::string> tagList;
	inline std::vector<Layer> layerList;

	inline GameObject* selectedGO;
	inline std::vector<GameObject*> gameObjectList;

	inline sf::Texture cellTexture;

	inline GameObject* selectedTileMapGO;
	inline Grid::Cell* selectedCell;
	inline std::vector<GameObject*> tilemapList;

	inline static std::string currentPath = std::filesystem::current_path().string();
	inline static std::string selectedFile;

	inline bool openPaletteEditor;
};