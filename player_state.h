#pragma once
#include "common.h"


enum class EPlayer_Speed_Recharge_Substate : unsigned char
{
	None,
	Stage_1,
	Stage_2,
};

//=====================================================================================================================
enum class EPlayer_Speed_Substate : unsigned char
{
	None,
	Stage_1,
	Stage_2,
};

//=====================================================================================================================
enum class EPlayer_Hit_Substate : unsigned char
{
	None,
	Stage_1,
	Stage_2,
};

//=====================================================================================================================
enum class EPlayer_Recharge_Substate : unsigned char
{
	None,
	Stage_1,
	Stage_2,
};

//=====================================================================================================================

enum class EPlayer_State : unsigned char
{
	Is_Noramal,
	Is_Dead,
	Recharge,
	Hit,
	Speed_Effect,
	Speed_Recharge_Effect,
	Repair,
};

//=====================================================================================================================
class AsPlayer_State
{
public:
	~AsPlayer_State();
	AsPlayer_State();

	void Init(sf::Sprite* player_spite, float*player_speed, int *player_hp);
	void Reset_Settings();

	operator EPlayer_State() const;
	void operator=(EPlayer_State new_state);

	void Update();
	bool Is_Recharge_Finished();

private:
	bool Recharge_State;
	bool Hit_State;

	void Recharge_Update();
	void Hit_Update();
	void Speed_Effect_Update();
	void Speed_Recharge_Effect_Update();

	EPlayer_Hit_Substate Hit_Substate;
	EPlayer_Recharge_Substate Recharge_Substate;
	EPlayer_Speed_Substate Speed_Substate;
	EPlayer_Speed_Recharge_Substate Speed_Recharge_Substate;

	const float Recharge_Time_Const = 2.f;
	float Recharge_Timer;
	float Recharge_Time_Limit = Recharge_Time_Const;

	float Hit_Time;
	float Hit_Time_Limit;

	float *Player_Speed;
	float Speed_Effect_Timer;
	const float Speed_Effect_Time_Limit = 20.f;

	int* Player_Hp;

	float Speed_Recharge_Effect_Timer;
	const float Speed_Recharge_Time_Limit = 12.f;

	EPlayer_State Last_State;

	sf::Sprite* Player_Spite;
	sf::Color Player_Sprite_Color;

	
};