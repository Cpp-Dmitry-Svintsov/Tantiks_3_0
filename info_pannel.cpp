#include "info_pannel.h"

//===============================================================================================================================
// constructor & destructor
//===============================================================================================================================
AsInfo_Pannel::~AsInfo_Pannel()
{}
AsInfo_Pannel::AsInfo_Pannel()
	:Player_Hp(0), Level(1), Player_Base_Hp(0), Enemy_Base_Hp(0), Lives(0)
{
}
void AsInfo_Pannel::Init(AEngine_Interface_For_Info_Pannel* engine)
{
	Engine = engine;

	if (!Font.loadFromFile("fonts/Jersey.ttf"))
		printf("coud not load font jersey.ttf for info pannel");
	Text.setFont(Font);
	Text.setCharacterSize(45);
	Text.setOutlineColor(sf::Color::Blue);
	Text.setFillColor(sf::Color::White);

	Text.setPosition(sf::Vector2f(10.f, (float)(AsSettings::Level_Height * AsSettings::Ceil_Size) ));
}


//===============================================================================================================================
// public secton:
//===============================================================================================================================
void AsInfo_Pannel::Update()
{
	Engine->Get_Info(Player_Hp, Level, Player_Base_Hp, Enemy_Base_Hp, Lives);
}
//===============================================================================================================================
void AsInfo_Pannel::Draw(sf::RenderWindow& window)
{
	std::stringstream my_str;
	my_str.str(" ");

	my_str << "Player HP - " << Player_Hp << "          " << "Level - " << Level << "          " << "LIVES -> " << Lives << "\n";
	my_str << "  " << "Player Base HP - " << Player_Base_Hp << "         " << "Enemy Base HP - " << Enemy_Base_Hp;

	Text.setString(my_str.str() );

	window.draw(Text);
}
//===============================================================================================================================