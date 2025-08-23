#include "MonoBehaviour.h"

MonoBehaviour::~MonoBehaviour()
{
	delete gameObject;
}

void MonoBehaviour::Start()
{
}

void MonoBehaviour::Update(float _dt)
{
}

void MonoBehaviour::Cleanup()
{
}
