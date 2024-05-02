#pragma once
#include "common.h"

enum class EPlay_Level_Substate : unsigned char
{
	None,
	Stage_1,
	Stage_2,
};

//===============================================================================================================================

enum class EGame_State : unsigned char
{
	Menu,
	Play_Current_Level,
	Constructor
};
//===============================================================================================================================
class AsGame_State
{
public:
	~AsGame_State();
	AsGame_State();

	void Init();

	operator EGame_State() const;
	void operator=(EGame_State new_state);

	void Update();
	void Draw(sf::RenderWindow& window);

private:
	void Start_Level_Update();
	void Draw_Play_Level_Substate(sf::RenderWindow& window);
	void Restore_Settings();

	EPlay_Level_Substate Play_Level_Substate;

	EGame_State Main_State;

	sf::RectangleShape Rect_Top, Rect_Right, Rect_Bottom, Rect_Left;
	sf::Color Fill_Color;

	float Screen_Width, Screen_Height;
	const float Margin = 50.f;

	float Speed;
};
