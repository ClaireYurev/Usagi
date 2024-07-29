#include "Timer.h"

using namespace std::chrono;

UsagiTimer::UsagiTimer()
{
	last = steady_clock::now();
}

float UsagiTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float UsagiTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
