#pragma once
#include "common.h"

//=====================================================================================================================
class AItem
{
public:
	~AItem();
	AItem(sf::Vector2f pos, EItem_Type type);

	void Update();
	void Draw(sf::RenderWindow &window);

	void Take_Item(EItem_Type &type);
	bool Is_Dead();

	sf::Vector2i Get_Pos();

private:
	static int Count_Of_Items;
	static sf::Texture *Texture;

	bool Is_Alive;

	const int Time_Tol_Live_Limit = 30000;
	int Time_To_Live;

	EItem_Type Item_Type;

	sf::Sprite Sprite;

	sf::Vector2i Pos;
};