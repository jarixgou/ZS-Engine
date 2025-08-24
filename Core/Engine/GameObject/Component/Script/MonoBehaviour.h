#pragma once
#include "../../GameObject.h"

typedef class MonoBehaviour
{
public:
	virtual ~MonoBehaviour();

	virtual void Start();
	virtual void Update(float _dt);
	virtual void Cleanup();

	bool enable = true;
	ZS::GameObject* gameObject = nullptr;
}MonoBehaviour;