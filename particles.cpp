#include "particles.h"

//=================================================================================================================================
// class Particle & projectTile
//=================================================================================================================================
// static mamber declarated
//=================================================================================================================================
int AParticle::Count = 0;
sf::Texture* AParticle::Texture = nullptr;
//=================================================================================================================================
// cnstructor & destructor
//=================================================================================================================================
AParticle::~AParticle()
{
	--Count;
	if (Count <= 0)
		if (AParticle::Texture != nullptr)
		{
			delete AParticle::Texture;
			AParticle::Texture = nullptr;
		}
}
AParticle::AParticle(EParticles_Type particle_type, sf::Vector2f pos, EDirection direction)
	:Is_Alive(true), X_Offset(0.f), Particle_Type(particle_type)
{
	++Count;
	if (AParticle::Texture == nullptr)
		Load_Media();

	Sprite.setTexture(*AParticle::Texture);
	switch (direction)
	{
	case EDirection::Top:
		pos.y -= 25.f;
		break;
	case EDirection::Right:
		pos.x += 25.f;
		break;
	case EDirection::Bottom:
		pos.y += 25.f;
		break;
	case EDirection::Left:
		pos.x -= 25.f;
		break;
	default:
		break;
	}
	Sprite.setPosition(pos);
}
//=================================================================================================================================
AParticle::AParticle(sf::Vector2f pos, EParticles_Type particle_type)
	:Is_Alive(true), X_Offset(0.f), Particle_Type(particle_type)
{
	++Count;
	Sprite.setTexture(*AParticle::Texture);
	Sprite.setPosition(pos);
}
//=================================================================================================================================
AParticle::AParticle(sf::Vector2f pos, EDirection direction)
	:Is_Alive(true), Position(pos), Direction(direction), kill_Timer_Cheker(0), Speed(7.f)
{
	++Count;
	float f_ceil_size = (float)AsSettings::Ceil_Size / 2.f;
	sf::Vector2f orign(2.f, 2.f);

	Shape.setRadius(4.f);
	Shape.setFillColor(sf::Color(130, 0, 20, 255));

	switch (direction)
	{
	case EDirection::Top:
		Position.y -= 5.f;
		Position.x += 25.f;
		break;
	case EDirection::Right:
		Position.x += 55.f;
		Position.y += 25.f;
		break;
	case EDirection::Bottom:
		Position.y += 55.f;
		Position.x += 25.f;
		break;
	case EDirection::Left:
		Position.x -= 5.f;
		Position.y += 25.f;
		break;
	default:
		break;
	}

	Shape.setOrigin(orign);
	Shape.setPosition(Position + orign);
}
//=================================================================================================================================
// public methods
//=================================================================================================================================
void AParticle::Update()
{
	X_Offset += 0.3f * AsSettings::Get_Delta_Time();
	if (X_Offset >= 8.f)
	{
		Is_Alive = false;
	}
}
//=================================================================================================================================
void AParticle::Moove()
{
	++kill_Timer_Cheker;
	float dt = AsSettings::Get_Delta_Time();

	switch (Direction)
	{
	case EDirection::Top:
		Position.y -= Speed * dt;
		break;
	case EDirection::Right:
		Position.x += Speed * dt;
		break;
	case EDirection::Bottom:
		Position.y += Speed * dt;
		break;
	case EDirection::Left:
		Position.x -= Speed * dt;
		break;
	default:
		break;
	}
	Shape.setPosition(Position);

	//kill the instance
	if (kill_Timer_Cheker > 3500)
		Is_Alive = false;
}
//=================================================================================================================================
void AParticle::Draw(sf::RenderWindow& window)
{
	int y_offset = (int)Particle_Type;
	int ceil_size = AsSettings::Ceil_Size;

	Sprite.setTextureRect(sf::IntRect((int)X_Offset * ceil_size, y_offset * ceil_size, ceil_size, ceil_size));
	window.draw(Sprite);
}
//=================================================================================================================================
void AParticle::Draw_ProjectTile(sf::RenderWindow& window)
{
	window.draw(Shape);
}
//=================================================================================================================================
void AParticle::Delete_Instance()
{
	Is_Alive = false;
}
//=================================================================================================================================
bool AParticle::Get_State()
{
	return Is_Alive;
}
//=================================================================================================================================
sf::Vector2f AParticle::Get_Position(EDirection& direction)
{
	direction = Direction;
	return Position;
}
//=================================================================================================================================
// private section
//=================================================================================================================================
void AParticle::Load_Media()
{
	Texture = new sf::Texture();
	Texture->loadFromFile("textures/explosions.png");
}
//=================================================================================================================================
