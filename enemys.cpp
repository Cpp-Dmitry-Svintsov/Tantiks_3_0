#include "enemys.h"

int AEnemy::Count_Of_Enemys = 0;
sf::Texture* AEnemy::Texture = nullptr;
//========================================================================================================================
// constructor & destructor
//========================================================================================================================
AEnemy::~AEnemy()
{
	--Count_Of_Enemys;
	if (Count_Of_Enemys <= 0)
	{
		delete AEnemy::Texture;
		Texture = nullptr;
	}
}
AEnemy::AEnemy(int x, int y, EEnemys_Type enemy_type, EsLevel_Interface_For_Enemy* level_interface)
	:X_Offset(0), Direction(EDirection::Bottom), Speed(0.f), Hp(0), Enemy_Type(enemy_type), Dx_Mooving(0.f), Dy_Mooving(0.f),
	Enemy_Target(EEnemy_Target::Player), Level_Interface(level_interface)
{
	bool shoot_limit{};

	++Count_Of_Enemys;
	if (AEnemy::Texture == nullptr)
	{
		Texture = new sf::Texture();
		Texture->loadFromFile("textures/enemys.png");
	}
	Sprite.setTexture(*AEnemy::Texture);
	Sprite.setScale(sf::Vector2f(0.9f, 0.9f));

	Y_Offset = (int)enemy_type;

	//rotate sprite in direction and set position
	Sprite.setOrigin(sf::Vector2f(25.f, 25.f));
	Sprite.setPosition(Sprite.getOrigin().x + (float)(AsSettings::Ceil_Size * x),
		Sprite.getOrigin().y + (float)(AsSettings::Ceil_Size * y));
	Sprite.setRotation(180.f);

	switch (enemy_type)
	{
	case EEnemys_Type::Type_A:
		Speed = 0.6f;
		Hp = 2;
		shoot_limit = 3.f;
		break;
	case EEnemys_Type::Type_B:
		Speed = 0.7f;
		Hp = 3;
		shoot_limit = 2.f;
		break;
	case EEnemys_Type::Type_C:
		Speed = 0.9f;
		Hp = 2;
		shoot_limit = 1.5f;
		break;
	case EEnemys_Type::Type_D:
		Speed = 0.5f;
		Hp = 5;
		shoot_limit = 1.5f;
		break;
	default:
		break;
	}

	//init
	Enemy_State.Init(&Sprite, shoot_limit);
	Enemy_State = EEnemy_State::Normal;

	Mooving_Target.x = (float)x;
	Mooving_Target.y = (float)y;
}
//========================================================================================================================
// operatros redifinition:
//========================================================================================================================
AEnemy::operator EEnemy_State() const
{
	return Enemy_State;
}
void AEnemy::operator=(EEnemy_State new_state)
{
	Enemy_State = new_state;
}
//========================================================================================================================
// public sectin:
//========================================================================================================================
void AEnemy::Update()
{
	Enemy_State.Update();

	Mooving_Check();

	if ( Enemy_State.Is_Mooving_Substate() )
	{
		Moove();

		//animation on
		X_Offset += 0.3f * AsSettings::Get_Delta_Time();
		if (X_Offset >= 4.f)
			X_Offset = 0.f;
	}
}
//========================================================================================================================
void AEnemy::Draw(sf::RenderWindow& window)
{
	Sprite.setTextureRect(sf::IntRect((int)X_Offset * AsSettings::Ceil_Size, Y_Offset * AsSettings::Ceil_Size, AsSettings::Ceil_Size, AsSettings::Ceil_Size));
	window.draw(Sprite);
}
//========================================================================================================================
sf::Vector2f AEnemy::Get_Pos()
{
	return Sprite.getPosition();
}
//========================================================================================================================
void AEnemy::Hit()
{
	Enemy_State = EEnemy_State::Hit;
	--Hp;
	if (Hp <= 0)
		Enemy_State = EEnemy_State::Dead;
}
//========================================================================================================================
AEnemy_State *AEnemy::Get_State()
{
	return &Enemy_State;
}
//========================================================================================================================
EEnemys_Type AEnemy::Get_Type()
{
	return Enemy_Type;
}
//========================================================================================================================
std::vector<SCoord>* AEnemy::Get_Path_Vector()
{
	return &Path;
}
//========================================================================================================================
EEnemy_Target AEnemy::Get_Target()
{
	return Enemy_Target;
}
//========================================================================================================================
sf::Vector2f AEnemy::Get_Pos(EDirection& direction)
{
	direction = Direction;
	return Sprite.getPosition();
}
//========================================================================================================================
void AEnemy::Set_Pos(int x, int y)
{
	sf::Vector2f pos((float)(x * AsSettings::Ceil_Size), (float)(y * AsSettings::Ceil_Size));
	Sprite.setPosition(pos);
}
//========================================================================================================================
sf::Sprite& AEnemy::Get_Sprite()
{
	return Sprite;
}
//========================================================================================================================
// private section:
//========================================================================================================================
void AEnemy::Mooving_Check()
{
	SCoord next_ceil;
	int size = Path.size();
	sf::Vector2f pos{};
	float ceil_size = (float)AsSettings::Ceil_Size;
	float ax{}, by{};



	if (!Enemy_State.Is_Mooving_Substate() )
		if (size > 1)
		{
			pos = Sprite.getPosition();

			next_ceil = Path.at(size - 1);
			//check if ceil is free for other_ enemys
			if (Level_Interface->Check_Ceil_To_Moovable(next_ceil))
				return;

			Mooving_Target.x = (float)next_ceil.X;
			Mooving_Target.y = (float)next_ceil.Y;
			ax = floorf(pos.x / ceil_size);
			by = floorf(pos.y / ceil_size);
			X_Speed_Offset = Mooving_Target.x - ax;
			Y_Speed_Offset = Mooving_Target.y - by;
			Set_Direction();

			//check if ceil is occuped static object
			if (Level_Interface->Check_Static_Object(this, next_ceil))
				return;

			Path.pop_back();
			Enemy_State.Set_Mooving_Substate(true);
		}
}
//========================================================================================================================
void AEnemy::Moove()
{
	sf::Vector2f new_pos;
	float ceil_size{};
	float speed_dt = Speed * AsSettings::Get_Delta_Time();
	//float speed_dt = 0.2;
	float dx{}, dy{};

	ceil_size = (float)AsSettings::Ceil_Size;

	new_pos = Sprite.getPosition();
	
	Dx_Mooving += fabs(speed_dt * X_Speed_Offset);
	Dy_Mooving += fabs(speed_dt * Y_Speed_Offset);

	new_pos.x += speed_dt * X_Speed_Offset;
	new_pos.y += speed_dt * Y_Speed_Offset;


	if (Dx_Mooving > ceil_size || Dy_Mooving > ceil_size)
	{
		Enemy_State.Set_Mooving_Substate(false);
		new_pos.x = Mooving_Target.x * ceil_size;
		new_pos.y = Mooving_Target.y * ceil_size;
		Sprite.setPosition(new_pos + Sprite.getOrigin());
		Dx_Mooving = 0.f;
		Dy_Mooving = 0.f;
	}
	else
		Sprite.setPosition(new_pos);
}
//========================================================================================================================
void AEnemy::Set_Direction()
{
	float angle = 0.f;

	if (X_Speed_Offset == 0.f && Y_Speed_Offset == -1.f)
		Direction = EDirection::Top;
	else if (X_Speed_Offset == 1.f && Y_Speed_Offset == 0.f)
		Direction = EDirection::Right;
	else if (X_Speed_Offset == 0.f && Y_Speed_Offset == 1.f)
		Direction = EDirection::Bottom;
	else if (X_Speed_Offset == -1.f && Y_Speed_Offset == 0.f)
		Direction = EDirection::Left;

	angle = (float)Direction * 90.f;
	Sprite.setRotation(angle);
}
//========================================================================================================================
