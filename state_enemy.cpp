#include "state_enemy.h"

//========================================================================================================================
// constructor & destructor & init
//========================================================================================================================
AEnemy_State::~AEnemy_State()
{

}
//========================================================================================================================
AEnemy_State::AEnemy_State()
	:Hit_Substate(EEnemy_Hit_Substate::None), Sprite_Color(sf::Color::White),
	Hit_Timer(0.f), Shoot_Timer(0.f), Hit_Time_Limit(0.5f), Shoot_Time_Limit(3.f), Mooving_Substate(false), Hit_State(false), Shoot_State(false)
{
}
//========================================================================================================================
void AEnemy_State::Init(sf::Sprite* sprite, float shoot_limit)
{
	Sprite = sprite;
	Sprite_Color = Sprite->getColor();
	Shoot_Time_Limit = shoot_limit;
}

// ==================================================================================================================
// operator redifinition
//========================================================================================================================
AEnemy_State::operator EEnemy_State() const
{
	return Last_State;
}
//========================================================================================================================
void AEnemy_State::operator=(EEnemy_State new_state)
{
	switch (new_state)
	{
	case EEnemy_State::Normal:
		break;
	case EEnemy_State::Hit:
		Hit_State = true;
		Hit_Substate = EEnemy_Hit_Substate::Stage_1;
		break;
	case EEnemy_State::Dead:
		break;
	case EEnemy_State::Shooting:
		Shoot_State = true;
		break;
	default:
		break;
	}

	Last_State = new_state;
}

//========================================================================================================================
// public sectin:
//========================================================================================================================
void AEnemy_State::Update()
{
	if (Hit_State)
		Hit_Update();

	if (Shoot_State)
		Shoot_Update();

}
//========================================================================================================================
bool AEnemy_State::Is_Mooving_Substate()
{
	if (Mooving_Substate)
		return true;
	else
		return false;
}
//=======================================================================================================================
void AEnemy_State::Set_Mooving_Substate(bool state)
{
	Mooving_Substate = state;
}
//=======================================================================================================================
bool AEnemy_State::Is_Dead()
{
	if (Last_State == EEnemy_State::Dead)
		return true;
	else
		return false;
}
//=======================================================================================================================
bool AEnemy_State::Is_Shooting()
{
	if (Shoot_State)
		return true;
	else
		return false;
}
//=======================================================================================================================
// private section:
//========================================================================================================================
void AEnemy_State::Hit_Update()
{
	switch (Hit_Substate)
	{
	case EEnemy_Hit_Substate::None:
		Hit_Timer = 0.f;
		Sprite->setColor(Sprite_Color);
		Hit_State = false;
		break;
	case EEnemy_Hit_Substate::Stage_1:
		Sprite->setColor(sf::Color::Red);
		Hit_Substate = EEnemy_Hit_Substate::Stage_2;
		break;
	case EEnemy_Hit_Substate::Stage_2:
		Hit_Timer += 0.03f * AsSettings::Get_Delta_Time();
		if (Hit_Timer > Hit_Time_Limit)
			Hit_Substate = EEnemy_Hit_Substate::None;
		break;
	default:
		break;
	}
}
//========================================================================================================================
void AEnemy_State::Shoot_Update()
{
	Shoot_Timer += 0.03f * AsSettings::Get_Delta_Time();
	if (Shoot_Timer > Shoot_Time_Limit)
	{
		Shoot_Timer = 0.f;
		Shoot_State = false;
	}
}
//========================================================================================================================
