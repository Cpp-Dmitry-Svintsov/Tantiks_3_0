#pragma once
#include "common.h"

enum class  EParticles_Type : unsigned char
{
	Tank_Shoot1 = 0,
	Tank_Shoot2 = 1,
	Tank_Shoot3 = 2,
};
//=================================================================================================================================
// class Particles
//=================================================================================================================================
class AParticle
{
public:
	~AParticle();
	AParticle(EParticles_Type particle_type, sf::Vector2f pos, EDirection direction);
	AParticle(sf::Vector2f pos, EParticles_Type particle_type);
	AParticle(sf::Vector2f pos, EDirection direction);

	void Update();
	void Moove();
	void Draw(sf::RenderWindow& window);
	void Draw_ProjectTile(sf::RenderWindow& window);

	void Delete_Instance();
	bool Get_State();
	sf::Vector2f Get_Position(EDirection &direction);

private:
	int kill_Timer_Cheker; //Kill when time is 1000...
	void Load_Media();

	static sf::Texture *Texture;

	float Speed;

	sf::Sprite Sprite;
	sf::CircleShape Shape;

	sf::Vector2f Position;
	EDirection Direction;

	EParticles_Type Particle_Type;


	bool Is_Alive;

	float X_Offset;

	static int Count;
};
