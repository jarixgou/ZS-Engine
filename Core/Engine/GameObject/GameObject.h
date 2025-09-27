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

			const std::vector<Component*>* GetComponentList(void) const;

			template<class T>
			void AddComponent(void)
			{
				if (std::is_base_of_v<T, IComponentData>)
				{
					DEBUG_INFO("Component must inherit from IComponentData", RED);
					return;
				}

				for (const auto component : this->componentList)
				{
					if (std::is_same_v<Tilemap, T>)
					{
						DEBUG_INFO("You can't put the tilemap component because you already have one", RED);
						return;
					}

					if (std::is_same_v<Grid, T>)
					{
						DEBUG_INFO("You can't put the grid component because you already have one", RED);
						return;
					}
				}

				Component* component = new Component;
				std::unique_ptr<T> data = std::make_unique<T>();

				component->data = std::move(data);
				this->componentList.push_back(component);
			}

			template<class T>
			T* GetComponent(void)
			{
				if (std::is_base_of_v<T, IComponentData>)
				{
					DEBUG_INFO("Component must inherit from IComponentData", RED);
					return nullptr;
				}

				for (auto component : this->componentList)
				{
					if (auto* data = dynamic_cast<T*>(component->data.get()))
					{
						return data;
					}
				}
				return nullptr;
			}

			void ResizeGrid(const Grid& _grid, sf::Texture& _cellTexture, sf::Texture& _tilesetTexture);
		private:
			std::vector<Component*> componentList;
		}GameObject;
}