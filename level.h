#pragma once
#include "particles.h"
#include "player.h"
#include "enemys.h"
#include "path_finder.h"
#include "enemy_spauner.h"
#include "items.h"

class AsLevel : public ALevel_Collision_Cheker, public EsLevel_Interface_For_Enemy
{
public:
	~AsLevel();
	AsLevel();
	void Init(AsPlayer *player, EGame_Messages* messager);

	void Init_new_level(int level);
	void Restart_Level();
	void Update();
	void Draw(sf::RenderWindow& window);

	void Add_Explosion_Particle(AEnemy* obj, EDirection dir);
	void Get_Current_Level_Data_Info(int& cur_lvl_number, int& player_base_hp, int& enemy_base_hp);
	bool Next_Level();

	virtual bool Chek_Static_Collision(sf::Vector2f pos, EDirection dir);
	virtual void Add_Explosion_Particle(AObject_Get_Sprite *obj, bool is_shoot);

	virtual bool Check_Ceil_To_Moovable(SCoord ceil_pos);
	virtual bool Check_Static_Object(AEnemy* enemy_obj, SCoord next_ceil);

private:
	void Particles_Update();
	void Project_Tiles_Update();
	void Enemys_Update();
	void Items_Update();
	void Clear_Data();
	void Load_Level_Data(unsigned int new_level);
	void Construct_New_Level();
	void Check_Collosion_Project_Tile(AParticle *obj);
	void Check_Hit_Enemy(AParticle* obj);
	void Add_Exp_Particle(sf::Vector2f pos, EParticles_Type type);
	void Spauners_Act();
	void Add_Enemys(ASpauner* spauner, EEnemys_Type enemy_type);
	void Pathfinder_Act();
	void Prepare_To_Shooting(AEnemy *enemy_obj);
	void ProjectTile_Chek_Hit_Player(AParticle* obj);

	int Type_A_Count;
	int Type_B_Count;
	int Type_C_Count;
	int Type_D_Count;

	int Path_Finder_Indicator;

	float Timer;
	float Spaun_Time_Limit;

	std::vector<AParticle*> Particles; //explosion particles
	std::vector<AParticle*> ProjectTiles; //PrjectTiles
	std::vector<AEnemy*> Enemys; //Enemys
	std::vector<SCoord> Occupied_Ceils; //cells occupied by enemies
	std::vector<ASpauner*> Spauners; // Enemy spaun points !!! Invisible
	std::vector<AItem*> Items; //Items collection !! Spawn Random after destroy barrels

	int Cur_Level_Number;
	AStatic_Object* Static_Pointer_Object_Arr[AsSettings::Level_Height][AsSettings::Level_Width]{};

	AObject_Get_Sprite* Player;

	SLevel_Data Cur_Level_Data;

	APath_Finder Path_Finder;

	AStatic_Object *Player_Base;
	AStatic_Object *Enemy_Base;

	EGame_Messages *Game_Messages;

	sf::SoundBuffer Shoot_Buffer;
	sf::Sound Sound_Shoot;

	sf::SoundBuffer Enemy_Destroy_Buffer;
	sf::Sound Sound_Enemy_Destroy;

	sf::SoundBuffer Object_Destroy_Buffer;
	sf::Sound Sound_Object_Destroy;
};