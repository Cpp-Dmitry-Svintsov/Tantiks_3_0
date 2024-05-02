#pragma once
#include "common.h"


class AsMenu
{
public:
	~AsMenu();
	AsMenu();

	void Init(EGame_Messages* game_messeger);

	void Update(bool is_key_released);
	void Draw(sf::RenderWindow &window);

private:
	int Higlight_Pos;

	sf::Font Spider_Font;
	sf::Font Jersey_Font;

	sf::Text Hider;
	sf::Text Menu_List[3];

	EKey_Pressed Key_State;

	EGame_Messages *Game_Messsages;
};
