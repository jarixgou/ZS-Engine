#pragma once
#include <functional>
#include <string>
#include <unordered_map>

#include "framework.h"
#include "MonoBehaviour.h"

typedef class ScriptRegistry
{
public:
	using ScriptFactory = std::function<MonoBehaviour* ()>;
		
	static ScriptRegistry& GetInstance()
	{
		static ScriptRegistry instance;
		return instance;
	}

	void RegisterScript(const std::string& scriptName, ScriptFactory factory);

	MonoBehaviour* CreateScript(const std::string& scriptName);

	std::vector<std::string> GetRegisteredScriptNames() const;

private:
	std::unordered_map<std::string, ScriptFactory> registry;
}ScriptRegistry;


#define REGISTER_SCRIPT(ScriptClass) \
	extern "C" __declspec(dllexport) MonoBehaviour* Create##ScriptClass() { return new ScriptClass(); } \
	struct ScriptRegistrar##ScriptClass { \
		ScriptRegistrar##ScriptClass() { \
			ScriptRegistry::GetInstance().RegisterScript(#ScriptClass, Create##ScriptClass); \
		} \
	}; \