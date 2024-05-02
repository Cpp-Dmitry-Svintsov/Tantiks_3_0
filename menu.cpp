#include "menu.h"

//=====================================================================================================================
// constructor & destrucor & initialization
//=====================================================================================================================
AsMenu::~AsMenu()
{}
AsMenu::AsMenu()
	:Higlight_Pos(0), Key_State(EKey_Pressed::None)
{
	Game_Messsages = nullptr;
}
void AsMenu::Init(EGame_Messages *game_messeger)
{
	int i{};
	float wx{};
	sf::Vector2f pos{};

	Game_Messsages = game_messeger;

	Spider_Font.loadFromFile("fonts/spider.ttf");
	Jersey_Font.loadFromFile("fonts/jersey.ttf");

	Hider.setFont(Spider_Font);
	Hider.setCharacterSize(33);
	Hider.setFillColor(sf::Color::Red);
	Hider.setOutlineThickness(2);
	Hider.setOutlineColor(sf::Color::White);
	Hider.setString("menu:");
	wx = Hider.getLocalBounds().width;
	Hider.setPosition( sf::Vector2f( (float)AsSettings::Screen_Width / 2.0 -  wx / 2.0, 300.f) );

	wx;
	pos = sf::Vector2f(  (float)AsSettings::Screen_Width / 2.0 - wx / 2.0, 350.f  );

	for (i; i < sizeof(Menu_List) / sizeof(Menu_List[0]); ++i)
	{
		Menu_List[i].setFont(Jersey_Font);
		Menu_List[i].setCharacterSize(26);
		Menu_List[i].setFillColor(sf::Color::White);
		Menu_List[i].setPosition(pos);
		pos.y += 30.f;
	}

	Menu_List[0].setString("game");
	Menu_List[1].setString("constructor");
	Menu_List[2].setString("exit");
}

//=====================================================================================================================
// public section:
//=====================================================================================================================
void AsMenu::Update(bool is_key_released)
{
	int size = sizeof(Menu_List) / sizeof(Menu_List[0]);

	if (is_key_released)
		Key_State = EKey_Pressed::None;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && Key_State == EKey_Pressed::None )
	{
		--Higlight_Pos;
		Key_State = EKey_Pressed::Up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && Key_State == EKey_Pressed::None)
	{
		++Higlight_Pos;
		Key_State = EKey_Pressed::Down;
	}

	//enter
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) )
	{
		switch (Higlight_Pos)
		{
		case 0:
			*(Game_Messsages) = EGame_Messages::New_Game;
			break;
		case 1:
			*(Game_Messsages) = EGame_Messages::Constructor;
			break;
		case 2:
			*(Game_Messsages) = EGame_Messages::Exit;
			break;
		default:
			break;
		}
	}

	Higlight_Pos = std::min(std::max(Higlight_Pos, 0), size - 1);

}
//=====================================================================================================================
void AsMenu::Draw(sf::RenderWindow& window)
{
	int i{};

	window.draw(Hider);

	for (i; i < sizeof(Menu_List) / sizeof(Menu_List[0]); ++i)
	{
		if (i == Higlight_Pos)
		{
			Menu_List[i].setFillColor(sf::Color::Red);
			Menu_List[i].setCharacterSize(28);
		}
		else
		{
			Menu_List[i].setCharacterSize(26);
			Menu_List[i].setFillColor(sf::Color::White);
		}
		window.draw(Menu_List[i]);
	}
}
//=====================================================================================================================
