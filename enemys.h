#pragma once
#include "state_enemy.h"


enum class EEnemy_Target : unsigned char
{
	Player,
	Base
};

//========================================================================================================================

enum class EEnemys_Type : unsigned char
{
	Type_A,
	Type_B,
	Type_C,
	Type_D,
};

//========================================================================================================================
class AEnemy
{
public:
	~AEnemy();
	AEnemy(int x, int y, EEnemys_Type enemy_type, EsLevel_Interface_For_Enemy *level_interface);

	operator EEnemy_State() const;
	void operator=(EEnemy_State new_state);

	void Update();
	void Draw(sf::RenderWindow& window);

	sf::Vector2f Get_Pos();
	void Hit();
	AEnemy_State* Get_State();
	EEnemys_Type Get_Type();
	std::vector<SCoord>* Get_Path_Vector();
	EEnemy_Target Get_Target();


	sf::Vector2f Get_Pos(EDirection& direction);
	void Set_Pos(int x, int y);
	sf::Sprite& Get_Sprite();

private:
	void Mooving_Check();
	void Moove();
	void Set_Direction();

	static int Count_Of_Enemys;
	float X_Offset;
	float Speed;
	int Y_Offset;
	int Hp;

	EDirection Direction;
	AEnemy_State Enemy_State;
	EEnemys_Type Enemy_Type;

	static sf::Texture *Texture;
	sf::Sprite Sprite;
	std::vector<SCoord> Path;

	float X_Speed_Offset, Y_Speed_Offset;
	float Dx_Mooving, Dy_Mooving;
	sf::Vector2f Mooving_Target;

	EEnemy_Target Enemy_Target;

	EsLevel_Interface_For_Enemy* Level_Interface;
};