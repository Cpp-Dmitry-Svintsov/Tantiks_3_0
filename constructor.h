#pragma once
#include "common.h"

class AsConstructor
{
public:
	~AsConstructor();
	AsConstructor();

	void Update(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);

private:
	void Draw_Work_Space(sf::RenderWindow& window);
	void Draw_Objects(sf::RenderWindow& window);
	void Draw_Info_Pannel(sf::RenderWindow& window);
	void Save_Data();

	SLevel_Data Level_Data;
	//unsigned char Constructed_Level[AsSettings::Level_Height][AsSettings::Level_Width]{};

	unsigned char Selected_Element;

	//unsigned char Enemy_Count;
	//unsigned char Enemy_Type;

	sf::RectangleShape Cur_Lvl_Rect;
	sf::RectangleShape Pointer_Pos;//pointer to the selected element
};
