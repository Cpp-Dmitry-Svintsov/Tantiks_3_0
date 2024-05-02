#pragma once
#include "settings.h"

enum class EItem_Type : unsigned char
{
	Repair,
	Speed,
	Recharge,
};

//===============================================================================================================================
// pos elem for node elem structure
//===============================================================================================================================
struct SCoord
{
	SCoord();
	SCoord(int x, int y);

	bool operator<(const SCoord& l) const;
	bool operator==(const SCoord& other) const;
	bool operator!=(const SCoord& other) const;
	void operator=(const SCoord& other);
	void operator+(const SCoord& other);
	void operator+=(const SCoord& other);

	int X;
	int Y;
};
//===============================================================================================================================
// game messages
//===============================================================================================================================
enum class EGame_Messages : unsigned char
{
	None,
	Exit,
	Return_To_Menu,
	New_Game,
	Constructor,
	Next_Level,
	Restart_Level,
};

//===============================================================================================================================
// what key is pressed?
//===============================================================================================================================
enum class EKey_Pressed : unsigned char
{
	None,
	Up,
	Down,
	Right,
	Left,
};

//===============================================================================================================================
// structure for save_level data
//===============================================================================================================================
struct SLevel_Data
{
	int Limit_Type_A = 0;
	int Limit_Type_B = 0;
	int Limit_Type_C = 0;
	int Limit_Type_D = 0;
	unsigned char Level_Array[AsSettings::Level_Height][AsSettings::Level_Width]{};
};

//===============================================================================================================================
// direction enum
//===============================================================================================================================
enum class EDirection : unsigned char
{
	Top,
	Right,
	Bottom,
	Left
};

//===============================================================================================================================
// class interface for static object && enum
//===============================================================================================================================
enum class EStatic_Obj_Type : unsigned char
{
	None = 0,
	Brick = 1,
	Wall,
	Forest,
	Barrel,
	Enemy_Base,
	Player_Base,
	Enemy_Spauner,
	Player_Spauner,
};
//===============================================================================================================================
class AStatic_Object
{
public:
	virtual ~AStatic_Object();
	AStatic_Object(int x, int y, EStatic_Obj_Type obj_type);

	void Draw(sf::RenderWindow& window);
	bool Hit(EDirection dir);
	void Get_Start_Pos(sf::Vector2f &pos);
	void Get_Ceil_Pos(int& x, int& y);
	int Get_Hp();
	EStatic_Obj_Type Get_Type();

	sf::Vector2f Get_Pos();
	sf::Sprite& Get_Sprite();

private:
	static int Count_Of_Objects;

	int Hp;

	EStatic_Obj_Type Obj_Type;
	static sf::Texture* Texture;
	sf::Sprite Sprite;
	sf::IntRect Texure_Rect;

	sf::IntRect Start_Texture_Rect;
	sf::Vector2f Start_Pos;

	int CX;
	int CY;

};
class AObject_Get_Sprite;
//===============================================================================================================================
// interface collision cheker for level
//===============================================================================================================================
class ALevel_Collision_Cheker
{
public:
	virtual ~ALevel_Collision_Cheker();
	ALevel_Collision_Cheker();

	virtual bool Chek_Static_Collision(sf::Vector2f pos, EDirection dir) = 0;
	virtual void Add_Explosion_Particle(AObject_Get_Sprite* obj, bool is_shoot) = 0;
};
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================


//===============================================================================================================================
// interface get sprite for oject
//===============================================================================================================================
class AObject_Get_Sprite
{
public:
	virtual ~AObject_Get_Sprite();
	AObject_Get_Sprite();

	virtual sf::Vector2f Get_Pos(EDirection& direction) = 0;
	virtual void Set_Pos(int x, int y) = 0;
	virtual sf::Sprite& Get_Sprite() = 0;
	virtual void Hit() = 0;
	virtual void Set_State_Effect(EItem_Type effect) = 0;
};
//===============================================================================================================================

class AEnemy;
//===============================================================================================================================
// interface for Level -> to get fro every Enemy
//===============================================================================================================================
class EsLevel_Interface_For_Enemy
{
public:
	~EsLevel_Interface_For_Enemy();
	EsLevel_Interface_For_Enemy();

	virtual bool Check_Ceil_To_Moovable(SCoord ceil) = 0;
	virtual bool Check_Static_Object(AEnemy *enemy_obj, SCoord next_ceil) = 0;
};
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================


//===============================================================================================================================
// interface for engine
//===============================================================================================================================
class AEngine_Interface_For_Info_Pannel
{
public:
	~AEngine_Interface_For_Info_Pannel();
	AEngine_Interface_For_Info_Pannel();

	virtual void Get_Info(int &player_hp, int &cur_lvl_number, int &player_base_hp, int &enemy_base_hp, int &lives) = 0;
};

//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
