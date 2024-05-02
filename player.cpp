#include "player.h"

//=====================================================================================================================
// const & destructor & init
//=====================================================================================================================
AsPlayer::~AsPlayer()
{}
AsPlayer::AsPlayer()
	:Speed(2.f), Direction(EDirection::Top), Hp(5)
{
	Load_Media();
}
void AsPlayer::Init(ALevel_Collision_Cheker* lvl_cheker, EGame_Messages* messager)
{
	Level_Collision_Cheker = lvl_cheker;

	Player_State.Init(&Sprite, &Speed, &Hp);

	Game_Messages = messager;

	//sounds
	Shoot.loadFromFile("sounds/shoot_2.mp3");
	Shoot_Sound.setBuffer(Shoot);

	Hit_S.loadFromFile("sounds/hit.mp3");
	Hit_Sound.setBuffer(Hit_S);

	Pick_Item_S.loadFromFile("sounds/item_drop.mp3");
	Sound_Pick_Item.setBuffer(Pick_Item_S);
}
//=====================================================================================================================
//=====================================================================================================================
void AsPlayer::Update()
{
	bool is_key_pressed = false;
	sf::Vector2f new_pos = Sprite.getPosition();
	float dt = AsSettings::Get_Delta_Time();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !is_key_pressed)
	{
		is_key_pressed = true;
		new_pos.x -= Speed * dt;
		Sprite.setRotation(-90.f);
		Mooving_Animation();
		Direction = EDirection::Left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !is_key_pressed)
	{
		is_key_pressed = true;
		new_pos.x += Speed * dt;
		Sprite.setRotation(90.f);
		Mooving_Animation();
		Direction = EDirection::Right;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !is_key_pressed)
	{
		is_key_pressed = true;
		new_pos.y -= Speed * dt;
		Sprite.setRotation(0.f);
		Mooving_Animation();
		Direction = EDirection::Top;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !is_key_pressed)
	{
		is_key_pressed = true;
		new_pos.y += Speed * dt;
		Sprite.setRotation(180.f);
		Mooving_Animation();
		Direction = EDirection::Bottom;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && Player_State.Is_Recharge_Finished() )
	{
		Level_Collision_Cheker->Add_Explosion_Particle(this, true);
		//recharge weapon
		Player_State = EPlayer_State::Recharge;
		Shoot_Sound.play();
	}

	if (is_key_pressed)
		if (Level_Collision_Cheker->Chek_Static_Collision(new_pos, Direction))
			Sprite.setPosition(new_pos);
		else
			Moove_Back_If_Collision(new_pos);

	Player_State.Update();
}
//=====================================================================================================================
void AsPlayer::Draw(sf::RenderWindow& window)
{
	int ceil_size = AsSettings::Ceil_Size;
	Sprite.setTextureRect(sf::IntRect( (int)X_Offset * ceil_size, Y_Offset * ceil_size, ceil_size, ceil_size));
	window.draw(Sprite);
}
//=====================================================================================================================
sf::Vector2f AsPlayer::Get_Pos()
{
	return Sprite.getPosition() - Sprite.getOrigin();
}
//=====================================================================================================================
int AsPlayer::Get_Hp()
{
	return Hp;
}
// vurtual methods subsection:
//=====================================================================================================================
sf::Vector2f AsPlayer::Get_Pos(EDirection& direction)
{
	direction = Direction;
	return Sprite.getPosition() - Sprite.getOrigin();
}
//=====================================================================================================================
void AsPlayer::Set_Pos(int x, int y)
{
	Player_State.Reset_Settings();
	Player_State = EPlayer_State::Is_Noramal;

	Hp = 5;
	float ceil_size = (float)AsSettings::Ceil_Size;

	Sprite.setOrigin(sf::Vector2f(25.f, 25.f));
	Sprite.setPosition(Sprite.getOrigin().x + ceil_size * (float)x,
		Sprite.getOrigin().y + ceil_size * (float)y);
}
//=====================================================================================================================
sf::Sprite& AsPlayer::Get_Sprite()
{
	return Sprite;
}
//=====================================================================================================================
void AsPlayer::Hit()
{
	--Hp;
	Player_State = EPlayer_State::Hit;

	Hit_Sound.play();

	if (Hp <= 0)
	{
		Player_State = EPlayer_State::Is_Dead;
		*Game_Messages = EGame_Messages::Restart_Level;
	}
}
//=====================================================================================================================
void AsPlayer::Set_State_Effect(EItem_Type effect)
{
	switch (effect)
	{
	case EItem_Type::Speed:
		Player_State = EPlayer_State::Speed_Effect;
		break;
	case EItem_Type::Repair:
		Player_State = EPlayer_State::Repair;
		break;
	case EItem_Type::Recharge:
		Player_State = EPlayer_State::Speed_Recharge_Effect;
		break;
	default:
		break;
	}

	Sound_Pick_Item.play();
}
//=====================================================================================================================
// private
//=====================================================================================================================
void AsPlayer::Load_Media()
{
	if (!Texture.loadFromFile("textures/player_tank.png"))
		printf("could not load player texture");
	else
	{
		Sprite.setTexture(Texture);
		Sprite.setScale(sf::Vector2f(0.9f, 0.9f));
	}
}
//=====================================================================================================================
void AsPlayer::Mooving_Animation()
{
	X_Offset += 0.3f * AsSettings::Get_Delta_Time();
	if (X_Offset >= 4.f)
		X_Offset = 0.f;
}
//=====================================================================================================================
void AsPlayer::Moove_Back_If_Collision( sf::Vector2f pos)
{
	float f_ceil_size = (float)AsSettings::Ceil_Size;
	float h_ceil_size = f_ceil_size / 2.f;
	float x = floorf(pos.x / f_ceil_size);
	float y = floorf(pos.y / f_ceil_size);

	switch (Direction)
	{
	case EDirection::Top:
		Sprite.setPosition(pos.x, floorf(pos.y / f_ceil_size) * f_ceil_size + h_ceil_size);
		break;
	case EDirection::Right:
		Sprite.setPosition(floorf(pos.x / f_ceil_size) * f_ceil_size + h_ceil_size, pos.y);
		break;
	case EDirection::Bottom:
		Sprite.setPosition(pos.x, floorf(pos.y / f_ceil_size) * f_ceil_size + h_ceil_size);
		break;
	case EDirection::Left:
		Sprite.setPosition(floorf(pos.x / f_ceil_size) * f_ceil_size + h_ceil_size, pos.y);
		break;
	default:
		break;
	}
}
//=====================================================================================================================