#pragma once

#include "../../Common.h"
#include "../Layer/Layer.h"
#include "Component/Component.h"
#include "Component/Grid/Grid.h"
#include "Component/Tilemap/Tilemap.h"

namespace ZS
{
		typedef class GameObject
		{
		public:
			typedef struct Transform
			{
				sf::Vector2f pos;
				float rotation;
				sf::Vector2f scale;
			}Transform;

			GameObject* parent;
			std::vector<GameObject*> child;

			std::string name;
			std::string* tag;
			Layer* layer;

			Transform transform;

			std::vector<Component*> componentList;

			void CreateRectangularTilemap(sf::Texture& _cellTexture, std::vector<GameObject*>& _tilemapList);
			
			template<typename T>
			T* GetComponent(void);

			void ResizeGrid(const Grid& _grid, sf::Texture& _cellTexture, sf::Texture& _tilesetTexture);
		}GameObject;
}