#include "level_data.h"

//=====================================================================================================================
// constuctor & destructor
//=====================================================================================================================
ALevel_Data::~ALevel_Data()
{}
ALevel_Data::ALevel_Data(unsigned char* lvl_arr, unsigned char limit, unsigned char type)
	:Enemy_Limit(limit), Enemy_Type(type)
{
	Level_Arr = lvl_arr;
}
//=====================================================================================================================
//=====================================================================================================================

//=====================================================================================================================