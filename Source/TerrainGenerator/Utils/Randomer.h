#pragma once

#include <random>
#include <chrono>

class Randomer
{
private:
	std::minstd_rand0 Generator;

public:
	Randomer() : Generator(std::chrono::system_clock::now().time_since_epoch().count() ^ time(nullptr))
	{
	}

	int GetRandom()
	{
		return Generator();
	}

	void SetSeed(int seed)
	{
		Generator.seed(seed);
	}
};
