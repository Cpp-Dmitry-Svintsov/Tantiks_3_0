#pragma once
#include "player_state.h"

//===============================================================================================================================
class AsPlayer : public AObject_Get_Sprite
{
public:
	~AsPlayer();
	AsPlayer();
	void Init(ALevel_Collision_Cheker* lvl_cheker, EGame_Messages* messager);

	void Update();
	void Draw(sf::RenderWindow &window);


	sf::Vector2f Get_Pos();
	int Get_Hp();

	virtual sf::Vector2f Get_Pos(EDirection& direction);
	virtual void Set_Pos(int x, int y);
	virtual sf::Sprite& Get_Sprite();
	virtual void Hit();
	virtual void Set_State_Effect(EItem_Type effect);

private:
	void Load_Media();
	void Mooving_Animation();
	void Moove_Back_If_Collision(sf::Vector2f pos);

	sf::Vector2f Position;
	sf::Texture Texture;
	sf::Sprite Sprite;

	int Hp;

	float Speed;

	float X_Offset{};
	int Y_Offset{};
	EDirection Direction;

	AsPlayer_State Player_State;

	ALevel_Collision_Cheker* Level_Collision_Cheker;

	EGame_Messages* Game_Messages;

	sf::SoundBuffer Shoot;
	sf::Sound Shoot_Sound;

	sf::SoundBuffer Hit_S;
	sf::Sound Hit_Sound;

	sf::SoundBuffer Pick_Item_S;
	sf::Sound Sound_Pick_Item;

};