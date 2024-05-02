#pragma once
#include "common.h"

class ASpauner
{
public:
	~ASpauner();
	ASpauner(int x, int y);

	sf::Vector2f Get_Pos();
	void Get_Pos(int& x, int& y);

private:
	sf::Vector2f Ceil_Pos;
	int Ceil_X;
	int Ceil_Y;
};

