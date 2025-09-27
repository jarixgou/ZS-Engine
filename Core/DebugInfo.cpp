#include "DebugInfo.h"

#define RESET   "\033[0m"

static std::vector<std::string> debugLog;

void PrintDebugInfo(const std::string& _text, const std::string& _color)
{
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	std::tm localTime;
	localtime_s(&localTime, &currentTime);

	std::ostringstream oss;
	oss << _color << "[" << std::put_time(&localTime, "%H:%M:%S") << "] - " << _text << RESET << "\n";
	std::string outPut = oss.str();

	std::cout << outPut;

	debugLog.push_back(outPut);
}