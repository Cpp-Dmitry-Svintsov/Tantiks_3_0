#include "player_state.h"

//=====================================================================================================================
// constructor & destrucor
//=====================================================================================================================
AsPlayer_State::~AsPlayer_State()
{}
//=====================================================================================================================
AsPlayer_State::AsPlayer_State()
	:Recharge_Timer(0.f), Hit_Time(0.f), Hit_Time_Limit(0.5f), Recharge_State(false), Hit_State(false),
	Last_State(EPlayer_State::Is_Noramal), Speed_Substate(EPlayer_Speed_Substate::None), Speed_Recharge_Substate(EPlayer_Speed_Recharge_Substate::None),
	Speed_Effect_Timer(0.f), Speed_Recharge_Effect_Timer(0.f)
{}
//=====================================================================================================================
void AsPlayer_State::Init(sf::Sprite* player_spite, float* player_speed, int* player_hp)
{
	Player_Spite = player_spite;
	Player_Sprite_Color = Player_Spite->getColor();

	Player_Speed = player_speed;
	Player_Hp = player_hp;
}
//=====================================================================================================================
void AsPlayer_State::Reset_Settings()
{
	Recharge_Time_Limit = Recharge_Time_Const;
}
//=====================================================================================================================
// operators redefinition
//=====================================================================================================================
AsPlayer_State::operator EPlayer_State() const
{
	return Last_State;
}
void AsPlayer_State::operator=(EPlayer_State new_state)
{
	switch (new_state)
	{
	case EPlayer_State::Is_Noramal:
		break;
	case EPlayer_State::Is_Dead:
		break;
	case EPlayer_State::Recharge:
		Recharge_State = true;
		break;
	case EPlayer_State::Hit:
		Hit_State = true;
		Hit_Substate = EPlayer_Hit_Substate::Stage_1;
		break;
	case EPlayer_State::Speed_Effect:
		Speed_Substate = EPlayer_Speed_Substate::Stage_1;
		break;
	case EPlayer_State::Speed_Recharge_Effect:
		Speed_Recharge_Substate = EPlayer_Speed_Recharge_Substate::Stage_1;
		break;
	case EPlayer_State::Repair:
		*Player_Hp = 5;
		break;
	default:
		break;
	}
}

//=====================================================================================================================
// public section
//=====================================================================================================================
void AsPlayer_State::Update()
{
	if (Recharge_State)
		Recharge_Update();

	if (Hit_State)
		Hit_Update();

	if (Speed_Substate != EPlayer_Speed_Substate::None)
		Speed_Effect_Update();

	if (Speed_Recharge_Substate != EPlayer_Speed_Recharge_Substate::None)
		Speed_Recharge_Effect_Update();

}
//=====================================================================================================================
bool AsPlayer_State::Is_Recharge_Finished()
{
	if (Recharge_State)
		return false;
	else
		return true;
}
//=====================================================================================================================
// private section:
//=====================================================================================================================
void AsPlayer_State::Recharge_Update()
{
	Recharge_Timer += 0.03f * AsSettings::Get_Delta_Time();
	if (Recharge_Timer > Recharge_Time_Limit)
	{
		Recharge_Timer = 0.f;
		Recharge_State = false;
	}
}
//=====================================================================================================================
void AsPlayer_State::Hit_Update()
{

	switch (Hit_Substate)
	{
	case EPlayer_Hit_Substate::None:
		Hit_Time = 0.f;
		Player_Spite->setColor(Player_Sprite_Color);
		Hit_State = false;
		break;
	case EPlayer_Hit_Substate::Stage_1:
		Player_Spite->setColor(sf::Color::Cyan);
		Hit_Substate = EPlayer_Hit_Substate::Stage_2;
		break;
	case EPlayer_Hit_Substate::Stage_2:
		Hit_Time += 0.02f * AsSettings::Get_Delta_Time();
		if (Hit_Time > Hit_Time_Limit)
			Hit_Substate = EPlayer_Hit_Substate::None;
		break;
	default:
		break;
	}
}
//=====================================================================================================================
void AsPlayer_State::Speed_Effect_Update()
{
	float dt = AsSettings::Get_Delta_Time();

	switch (Speed_Substate)
	{
	case EPlayer_Speed_Substate::None:
		break;
	case EPlayer_Speed_Substate::Stage_1:
		*Player_Speed *= 2.f;
		Speed_Substate = EPlayer_Speed_Substate::Stage_2;
		Speed_Effect_Timer = 0.f;
		break;
	case EPlayer_Speed_Substate::Stage_2:
		Speed_Effect_Timer += 0.02f * AsSettings::Get_Delta_Time();
		if (Speed_Effect_Timer > Speed_Effect_Time_Limit)
		{
			Speed_Effect_Timer = 0.f;
			*Player_Speed /= 2.f;
			Speed_Substate = EPlayer_Speed_Substate::None;
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================
void AsPlayer_State::Speed_Recharge_Effect_Update()
{
	float dt = AsSettings::Get_Delta_Time();

	switch (Speed_Recharge_Substate)
	{
	case EPlayer_Speed_Recharge_Substate::None:
		break;
	case EPlayer_Speed_Recharge_Substate::Stage_1:
		Speed_Recharge_Effect_Timer = 0.f;
		Recharge_Time_Limit /= 2.f;
		Speed_Recharge_Substate = EPlayer_Speed_Recharge_Substate::Stage_2;
		break;
	case EPlayer_Speed_Recharge_Substate::Stage_2:
		Speed_Recharge_Effect_Timer += 0.02f * AsSettings::Get_Delta_Time();
		if (Speed_Recharge_Effect_Timer > Speed_Recharge_Time_Limit)
		{
			Speed_Recharge_Effect_Timer = 0.f;
			Recharge_Time_Limit = Recharge_Time_Const;
			Speed_Recharge_Substate = EPlayer_Speed_Recharge_Substate::None;
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================
