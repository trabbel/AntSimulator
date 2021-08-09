#pragma once
#include <cstdint>


enum class Mode : uint32_t
{
	ToHome = 0,
	ToFood = 1,
	ToHell = 2,
	CounterPhr = 3,
};

enum AntTracingPattern
{
	RANDOM,
	FOOD,
	HOME
};