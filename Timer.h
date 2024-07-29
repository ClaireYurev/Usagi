#pragma once
#include <chrono>

class UsagiTimer
{
public:
	UsagiTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};