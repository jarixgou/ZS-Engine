#include "pch.h"
#include "API.h"

void ScriptRegistry::RegisterScript(const std::string& scriptName, ScriptFactory factory)
{
	registry[scriptName] = factory;
}

MonoBehaviour* ScriptRegistry::CreateScript(const std::string& scriptName)
{
	auto it = registry.find(scriptName);
	if (it != registry.end())
	{
		return it->second();
	}
	return nullptr;
}

std::vector<std::string> ScriptRegistry::GetRegisteredScriptNames() const
{
	std::vector<std::string> scriptNames;
	for (const auto& pair : registry)
	{
		scriptNames.push_back(pair.first);
	}
	return scriptNames;
}
