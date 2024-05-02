#pragma once
#include "common.h"

class AsInfo_Pannel
{
public:
	~AsInfo_Pannel();
	AsInfo_Pannel();

	void Init(AEngine_Interface_For_Info_Pannel *engine);

	void Update();
	void Draw(sf::RenderWindow& window);

private:
	AEngine_Interface_For_Info_Pannel* Engine;

	int Player_Hp;
	int Level;
	int Player_Base_Hp;
	int Enemy_Base_Hp;
	int Lives;

	sf::Font Font;
	sf::Text Text;

};