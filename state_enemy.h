#pragma once
#include "common.h"



//========================================================================================================================

enum class EEnemy_Hit_Substate : unsigned char
{
	None,
	Stage_1,
	Stage_2,
};

//========================================================================================================================
enum class EEnemy_State : unsigned char
{
	Normal,
	Hit,
	Dead,
	Shooting,
};

//========================================================================================================================
class AEnemy_State
{
public:
	~AEnemy_State();
	AEnemy_State();
	void Init(sf::Sprite* sprite, float shoot_limit);

	operator EEnemy_State() const;
	void operator=(EEnemy_State new_state);

	void Update();

	bool Is_Mooving_Substate();
	void Set_Mooving_Substate(bool state);
	bool Is_Dead();
	bool Is_Shooting();

private:
	void Hit_Update();
	void Shoot_Update();

	bool Mooving_Substate;
	bool Hit_State;
	bool Shoot_State;

	EEnemy_State Last_State;
	EEnemy_Hit_Substate Hit_Substate;

	float Hit_Timer;
	float Shoot_Timer;

	float Hit_Time_Limit;
	float Shoot_Time_Limit;

	sf::Color Sprite_Color;
	sf::Sprite *Sprite;
};
