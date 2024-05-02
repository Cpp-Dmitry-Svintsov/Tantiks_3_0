#include "game_state.h"

//===============================================================================================================================
// constructor & destructor
//===============================================================================================================================
AsGame_State::~AsGame_State()
{
}
AsGame_State::AsGame_State()
	: Play_Level_Substate(EPlay_Level_Substate::None), Speed(2.f)
{}

//===============================================================================================================================
// operators redifinition:
//===============================================================================================================================
AsGame_State::operator EGame_State() const
{
	return Main_State;
}
void AsGame_State::operator=(EGame_State new_state)
{

	switch (new_state)
	{
	case EGame_State::Menu:
		break;
	case EGame_State::Play_Current_Level:
		Play_Level_Substate = EPlay_Level_Substate::Stage_1;
		break;
	case EGame_State::Constructor:
		break;
	default:
		break;
	}

	Main_State = new_state;
}

//===============================================================================================================================
// public section:
//===============================================================================================================================
void AsGame_State::Init()
{
	Fill_Color = sf::Color(40, 40, 40);

	Restore_Settings();

	Rect_Top.setFillColor(Fill_Color);
	Rect_Right.setFillColor(Fill_Color);
	Rect_Bottom.setFillColor(Fill_Color);
	Rect_Left.setFillColor(Fill_Color);
}
//===============================================================================================================================
void AsGame_State::Update()
{
	if (Play_Level_Substate != EPlay_Level_Substate::None)
		Start_Level_Update();
}
//===============================================================================================================================
void AsGame_State::Draw(sf::RenderWindow& window)
{
	if (Play_Level_Substate != EPlay_Level_Substate::None)
		Draw_Play_Level_Substate(window);
}
//===============================================================================================================================
// private sectin:
//===============================================================================================================================
void AsGame_State::Start_Level_Update()
{
	sf::Vector2f sh1, sh2, sh3, sh4;
	sf::Vector2f pos2, pos3;
	float dt = AsSettings::Get_Delta_Time();

	switch (Play_Level_Substate)
	{
	case EPlay_Level_Substate::Stage_1:
		sh1 = Rect_Top.getSize();
		sh1.y -= Speed * dt;
		Rect_Top.setSize(sh1);

		sh2 = Rect_Right.getSize();
		sh2.x -= Speed * dt;
		Rect_Right.setSize(sh2);
		pos2 = Rect_Right.getPosition();
		pos2.x += Speed * dt;
		Rect_Right.setPosition(pos2);

		sh3 = Rect_Bottom.getSize();
		sh3.y -= Speed * dt;
		Rect_Bottom.setSize(sh3);
		pos3 = Rect_Bottom.getPosition();
		pos3.y += Speed * dt;
		Rect_Bottom.setPosition(pos3);

		sh4 = Rect_Left.getSize();
		sh4.x -= Speed * dt;
		Rect_Left.setSize(sh4);

		if (sh1.y < 10.f)
		{
			Play_Level_Substate = EPlay_Level_Substate::None;
			Restore_Settings();
		}

		break;
	case EPlay_Level_Substate::Stage_2:
		break;
	default:
		break;
	}
}
//===============================================================================================================================
void AsGame_State::Draw_Play_Level_Substate(sf::RenderWindow& window)
{

	window.draw(Rect_Top);
	window.draw(Rect_Right);
	window.draw(Rect_Bottom);
	window.draw(Rect_Left);
}
//===============================================================================================================================
void AsGame_State::Restore_Settings()
{
	Screen_Width = (float)(AsSettings::Level_Width * AsSettings::Ceil_Size);
	Screen_Height = (float)(AsSettings::Level_Height * AsSettings::Ceil_Size);

	Rect_Top.setSize(sf::Vector2f(Screen_Width, Screen_Height / 2.f - Margin));
	Rect_Bottom.setSize(sf::Vector2f(Screen_Width, Screen_Height / 2.f - Margin));

	Rect_Right.setSize(sf::Vector2f(Screen_Width / 2.f - Margin, Screen_Height));
	Rect_Left.setSize(sf::Vector2f(Screen_Width / 2.f - Margin, Screen_Height));

	Rect_Bottom.setPosition(sf::Vector2f(0.f, Screen_Height / 2.f + Margin));
	Rect_Right.setPosition(sf::Vector2f(Screen_Width / 2.f + Margin, 0.f));
}
