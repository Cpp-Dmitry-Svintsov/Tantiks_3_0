#pragma once
#include "level_arrs.h"

class ALevel_Data
{
public:
	~ALevel_Data();
	ALevel_Data(unsigned char *lvl_arr, unsigned char limit, unsigned char type);

	unsigned char *Level_Arr;
	unsigned char Enemy_Limit;
	unsigned char Enemy_Type;
};
