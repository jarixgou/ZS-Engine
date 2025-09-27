#pragma once

typedef class MonoBehaviour
{
public:
	virtual ~MonoBehaviour();

	virtual void Start();
	virtual void Update(float _dt);
	virtual void Cleanup();

	bool enable = true;
}MonoBehaviour;
