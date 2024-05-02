#pragma once
#include "common.h"

class AsLevels_Arrays
{
protected:
	~AsLevels_Arrays();
	AsLevels_Arrays();

public:
	AsLevels_Arrays(AsLevels_Arrays& other) = delete;
	void operator=(AsLevels_Arrays&) = delete;

	static unsigned char Level_Test[AsSettings::Level_Height][AsSettings::Level_Width];
	static unsigned char Level_1[AsSettings::Level_Height][AsSettings::Level_Width];
};