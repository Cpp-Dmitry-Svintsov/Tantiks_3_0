#pragma once
#include "common.h"


//===============================================================================================================================
// Brick Walls
//===============================================================================================================================
class ABrick : public AStatic_Object
{
public:
	~ABrick();
	ABrick(int x, int y, EStatic_Obj_Type obj_type);

	void Hit(EDirection dir);
};