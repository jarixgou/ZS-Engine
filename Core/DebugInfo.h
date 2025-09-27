#pragma once
#include <string>
#include <Windows.h>
#include <chrono>
#include <iostream>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

void PrintDebugInfo(const std::string& _text, const std::string& _color = WHITE);

#define DEBUG_INFO(text, color) PrintDebugInfo(text, color)