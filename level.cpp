#include "level.h"

//===============================================================================================================================
// constructor & destructor & init
//===============================================================================================================================
AsLevel::~AsLevel()
{
	Clear_Data();
}
AsLevel::AsLevel()
	:Cur_Level_Number(0), Type_A_Count(0), Type_B_Count(0), Type_C_Count(0), Type_D_Count(0), Timer(0.f), Spaun_Time_Limit(3.f), Path_Finder_Indicator(0)
{
}
void AsLevel::Init(AsPlayer *player, EGame_Messages* messager)
{
	Player = player;

	Path_Finder.Init();

	Game_Messages = messager;

	Shoot_Buffer.loadFromFile("sounds/enemy_shoot.mp3");
	Sound_Shoot.setBuffer(Shoot_Buffer);

	Enemy_Destroy_Buffer.loadFromFile("sounds/enemy_destroy.mp3");
	Sound_Enemy_Destroy.setBuffer(Enemy_Destroy_Buffer);

	Object_Destroy_Buffer.loadFromFile("sounds/destroy_objects.mp3");
	Sound_Object_Destroy.setBuffer(Object_Destroy_Buffer);
	Sound_Object_Destroy.setVolume(25);
}
//===============================================================================================================================
// public methods
//===============================================================================================================================
void AsLevel::Init_new_level(int level)
{
	Clear_Data();
	Cur_Level_Number = level;

	//load new level
	Load_Level_Data(level);

	//factory of objects -> build current level
	Construct_New_Level();

	//build new grafs! for new_level
	Path_Finder.Clear_Data();
	Path_Finder.Get_Static_Graf( *(Cur_Level_Data.Level_Array) );
	Path_Finder.Get_Clear_Graf( *(Cur_Level_Data.Level_Array) );

	//clear all enemy_limits
	Type_A_Count = 0;
	Type_B_Count = 0;
	Type_C_Count = 0;
	Type_D_Count = 0;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!for debaging!!!!!!!!!!!!!!!!!!!!!!
	
	//Path_Finder.Print();
	//std::vector<SCoord> path;
	//Path_Finder.Get_Path(SCoord(3, 6), SCoord(15, 15), path);

	//Enemys.push_back(new AEnemy(3, 15, EEnemys_Type::Type_D));

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!for debaging - end!!!!!!!!!!!!!!!!!!!!!!

}
//===============================================================================================================================
void AsLevel::Restart_Level()
{
	Init_new_level(Cur_Level_Number);
}
//===============================================================================================================================
void AsLevel::Update()
{
	Occupied_Ceils.erase(Occupied_Ceils.begin(), Occupied_Ceils.end());

	Particles_Update();

	Project_Tiles_Update();

	Enemys_Update();

	Spauners_Act();

	Pathfinder_Act();

	Items_Update();
}
//===============================================================================================================================
void AsLevel::Draw(sf::RenderWindow& window)
{
	int i{}, j{};
	int ceil_size = AsSettings::Ceil_Size;
	int lvl_width = AsSettings::Level_Width;
	int lvl_heigt = AsSettings::Level_Height;
	AStatic_Object* cur_obj = nullptr;

	std::vector<AParticle*>::iterator particles_it;
	std::vector<AEnemy*>::iterator enemy_it;
	std::vector<AItem*>::iterator item_it;

	for (particles_it = Particles.begin(); particles_it < Particles.end(); ++particles_it)
		(*particles_it)->Draw(window);

	for (particles_it = ProjectTiles.begin(); particles_it < ProjectTiles.end(); ++particles_it)
		(*particles_it)->Draw_ProjectTile(window);

	for (item_it = Items.begin(); item_it != Items.end(); ++item_it)
		(*item_it)->Draw(window);

	for (enemy_it = Enemys.begin(); enemy_it < Enemys.end(); ++enemy_it)
		(*enemy_it)->Draw(window);

	for (i = 0; i < lvl_heigt; ++i)
		for (j = 0; j < lvl_width; ++j)
		{
			cur_obj = Static_Pointer_Object_Arr[i][j];
			if (cur_obj != nullptr)
				cur_obj->Draw(window);
		}


}
//===============================================================================================================================
void AsLevel::Add_Explosion_Particle(AEnemy* obj, EDirection dir)
{
	AParticle* particle;
	sf::Vector2f obj_pos;

	obj_pos = obj->Get_Sprite().getPosition() - obj->Get_Sprite().getOrigin();
	particle = new AParticle(EParticles_Type::Tank_Shoot1, obj_pos, dir);
	Particles.push_back(particle);

	particle = new AParticle(obj_pos, dir);
	ProjectTiles.push_back(particle);

}
//===============================================================================================================================
void AsLevel::Get_Current_Level_Data_Info(int& cur_lvl_number, int& player_base_hp, int& enemy_base_hp)
{
	cur_lvl_number = Cur_Level_Number;
	player_base_hp = Player_Base->Get_Hp();
	enemy_base_hp = Enemy_Base->Get_Hp();
}
//===============================================================================================================================
bool AsLevel::Next_Level()
{
	if (Cur_Level_Number >= AsSettings::Max_Level_Count)
		return false;
	else
	{
		Init_new_level(Cur_Level_Number + 1);
		return true;
	}
}
//virtual methods section:
//===============================================================================================================================
bool AsLevel::Chek_Static_Collision(sf::Vector2f pos, EDirection dir)
{
	EStatic_Obj_Type obj_type{};
	AStatic_Object* cur_obj = nullptr;
	int i{}, j{};
	int cx{}, cy{};
	int lvl_width{}, lvl_height{};
	sf::Vector2i col_start{};
	sf::Vector2i col_end{};

	lvl_width = AsSettings::Level_Width;
	lvl_height = AsSettings::Level_Height;

	if (pos.y + 22.f > (float)lvl_height * 50.f)
		return false;
	if (pos.y - 22.f < 0)
		return false;
	if (pos.x + 22.f > (float)lvl_width * 50.f)
		return false;
	if (pos.x - 22.f < 0)
		return false;


	cx = (int)(pos.x / 50.f);
	cy = (int)(pos.y / 50.f);

	col_start.x = cx > 0 ? cx - 1 : cx;
	col_end.x = cx < lvl_width - 1 ? cx + 1 : cx;

	col_start.y = cy > 0 ? cy - 1 : cy;
	col_end.y = cy < lvl_height - 1 ? cy + 1 : cy;

	switch (dir)
	{
	case EDirection::Top:
		col_end.y = col_start.y;
		break;
	case EDirection::Right:
		col_start.x = col_end.x;
		break;
	case EDirection::Bottom:
		col_start.y = col_end.y;
		break;
	case EDirection::Left:
		col_end.x = col_start.x;
		break;
	default:
		break;
	}

	for (i = col_start.y; i <= col_end.y; ++i)
		for (j = col_start.x; j <= col_end.x; ++j)
		{
			cur_obj = Static_Pointer_Object_Arr[i][j];
			if (cur_obj != nullptr)
			{
				obj_type = cur_obj->Get_Type();
				switch (obj_type)
				{
				case EStatic_Obj_Type::Brick:
				case EStatic_Obj_Type::Wall:
				case EStatic_Obj_Type::Barrel:
				case EStatic_Obj_Type::Enemy_Base:
				case EStatic_Obj_Type::Player_Base:
					if (Player->Get_Sprite().getGlobalBounds().intersects(Static_Pointer_Object_Arr[i][j]->Get_Sprite().getGlobalBounds()))
						return false;
				default:
					break;
				}
			}
		}

	return true;
}
//===============================================================================================================================
void AsLevel::Add_Explosion_Particle(AObject_Get_Sprite* obj, bool is_shoot)
{
	AParticle* particle;
	EDirection dir;
	sf::Vector2f obj_pos;

	obj_pos = obj->Get_Pos(dir);
	particle = new AParticle(EParticles_Type::Tank_Shoot1, obj_pos, dir);
	Particles.push_back(particle);

	if (is_shoot)
	{
		particle = new AParticle(obj_pos, dir);
		ProjectTiles.push_back(particle);
	}
}
//===============================================================================================================================
bool AsLevel::Check_Ceil_To_Moovable(SCoord ceil_pos)
{
	bool result{};
	std::vector<SCoord>::iterator it;

	for (it = Occupied_Ceils.begin(); it != Occupied_Ceils.end(); ++it)
	{
		if (*(it) == ceil_pos)
		{
			result = true;
			break;
		}
	}

	return result;
}
//===============================================================================================================================
bool AsLevel::Check_Static_Object(AEnemy* enemy_obj, SCoord next_ceil)
{
	EDirection dir{};

	sf::Vector2i ceil_pos = (sf::Vector2i)(enemy_obj->Get_Pos(dir) / (float)AsSettings::Ceil_Size);

	if ((EStatic_Obj_Type)Cur_Level_Data.Level_Array[next_ceil.Y][next_ceil.X] == EStatic_Obj_Type::Brick)
	{
		if (!enemy_obj->Get_State()->Is_Shooting())
		{
			*enemy_obj->Get_State() = EEnemy_State::Shooting;
			Add_Explosion_Particle(enemy_obj, dir);
		}
		return true;
	}

	return false;
}
//===============================================================================================================================
// private section
//===============================================================================================================================
void AsLevel::Particles_Update()
{
	std::vector<AParticle*>::iterator it;

	for (it = Particles.begin(); it < Particles.end(); ++it)
	{
		(*it)->Update();

		if (!(*it)->Get_State())
		{
			delete (*it);
			it = Particles.erase(it);
			if (it == Particles.end())
				break;
		}
	}
}
//===============================================================================================================================
void AsLevel::Project_Tiles_Update()
{
	std::vector<AParticle*>::iterator it;

	for (it = ProjectTiles.begin(); it < ProjectTiles.end(); ++it)
	{
		(*it)->Moove();

		Check_Collosion_Project_Tile((*it));

		Check_Hit_Enemy((*it));

		ProjectTile_Chek_Hit_Player((*it));

		if (!(*it)->Get_State())
		{
			delete (*it);
			it = ProjectTiles.erase(it);
			if (it == ProjectTiles.end())
				break;
		}
	}


}
//===============================================================================================================================
void AsLevel::Enemys_Update()
{
	sf::Vector2f sprite_enemy_pos;
	SCoord enemy_pos;
	std::vector<AEnemy*>::iterator enemy_it;
	float ceil_size{};
	sf::Vector2f original{};

	ceil_size = (float)AsSettings::Ceil_Size;

	for (enemy_it = Enemys.begin(); enemy_it != Enemys.end(); ++enemy_it)
	{
		(*enemy_it)->Update();

		sprite_enemy_pos = (*enemy_it)->Get_Pos();
		enemy_pos.X = (int)(sprite_enemy_pos.x / ceil_size);
		enemy_pos.Y = (int)(sprite_enemy_pos.y / ceil_size);
		Occupied_Ceils.push_back(enemy_pos);

		if ( (*enemy_it)->Get_State()->Is_Dead() )
		{
			Sound_Enemy_Destroy.play();

			original = sf::Vector2f(0.5f * ceil_size, 0.5f * ceil_size);

			Add_Exp_Particle((*enemy_it)->Get_Pos() - original, EParticles_Type::Tank_Shoot2);

			delete (*enemy_it);
			enemy_it = Enemys.erase(enemy_it);
			if (enemy_it == Enemys.end())
				break;

		}
	}

}
//===============================================================================================================================
void AsLevel::Items_Update()
{
	EItem_Type item_type{};
	std::vector<AItem*>::iterator it;
	sf::Vector2i player_pos;
	sf::Vector2i item_pos;

	player_pos = (sf::Vector2i)(Player->Get_Sprite().getPosition() / (float)AsSettings::Ceil_Size);

	for (it = Items.begin(); it != Items.end(); ++it)
	{
		(*it)->Update();

		item_pos = (*it)->Get_Pos();

		if (player_pos == item_pos)
		{
			(*it)->Take_Item(item_type);
			Player->Set_State_Effect(item_type);
		}

		if ((*it)->Is_Dead())
		{
			delete (*it);
			it = Items.erase(it);
			if (it == Items.end())
				break;
		}
	}
}
//===============================================================================================================================
void AsLevel::Clear_Data()
{
	AStatic_Object* cur_obj = nullptr;
	int i{}, j{};
	int ceil_size = AsSettings::Ceil_Size;
	int lvl_width = AsSettings::Level_Width;
	int lvl_heigt = AsSettings::Level_Height;

	//delete all particle
	for (auto* particle : Particles)
		delete particle;
	Particles.erase(Particles.begin(), Particles.end());

	//delete all projecttiles
	for (auto* project_tile : ProjectTiles)
		delete project_tile;
	ProjectTiles.erase( ProjectTiles.begin(), ProjectTiles.end() );

	//delete all enemys
	for (auto* enemy : Enemys)
		delete enemy;
	Enemys.erase(Enemys.begin(), Enemys.end());

	Occupied_Ceils.erase(Occupied_Ceils.begin(), Occupied_Ceils.end());

	//delete all spauners
	for (auto* spauner : Spauners)
		delete spauner;
	Spauners.erase(Spauners.begin(), Spauners.end());

	//delete all items
	for (auto* item : Items)
		delete item;
	Items.erase(Items.begin(), Items.end());

	//delete all static objects
	for (i = 0; i < lvl_heigt; ++i)
	{
		for (j = 0; j < lvl_width; ++j)
		{
			cur_obj = Static_Pointer_Object_Arr[i][j];
			if ( cur_obj != nullptr )
			{ 
				delete cur_obj;
				Static_Pointer_Object_Arr[i][j] = nullptr;
			}
		}
	}
}
//===============================================================================================================================
void AsLevel::Load_Level_Data(unsigned int new_level)
{
	std::ifstream infile;
	std::stringstream my_str;

	my_str.str("");
	my_str << "levels/level_" << new_level << ".dat";

	infile.open(my_str.str().c_str(), std::ios::binary | std::ios::in);

	infile.read(reinterpret_cast<char*>(&Cur_Level_Data), sizeof(Cur_Level_Data));

	infile.close();
}
//===============================================================================================================================
void AsLevel::Construct_New_Level()
{
	AStatic_Object* new_obj{};
	ASpauner* new_spauner{};
	int i{}, j{};
	int ceil_size = AsSettings::Ceil_Size;
	int lvl_width = AsSettings::Level_Width;
	int lvl_heigt = AsSettings::Level_Height;
	unsigned char ceil_data{};

	for (i = 0; i < lvl_heigt; ++i)
	{
		for (j = 0; j < lvl_width; ++j)
		{
			ceil_data = Cur_Level_Data.Level_Array[i][j];
			if (ceil_data != 0)
			{
				switch (ceil_data)
				{
				case 1:
					new_obj = new AStatic_Object(j, i, EStatic_Obj_Type::Brick);
					Static_Pointer_Object_Arr[i][j] = new_obj;
					break;
				case 2:
					new_obj = new AStatic_Object(j, i, EStatic_Obj_Type::Wall);
					Static_Pointer_Object_Arr[i][j] = new_obj;
					break;
				case 3:
					new_obj = new AStatic_Object(j, i, EStatic_Obj_Type::Forest);
					Static_Pointer_Object_Arr[i][j] = new_obj;
					break;
				case 4:
					new_obj = new AStatic_Object(j, i, EStatic_Obj_Type::Barrel);
					Static_Pointer_Object_Arr[i][j] = new_obj;
					break;
					//enemy base
				case 5:
					new_obj = new AStatic_Object(j, i, EStatic_Obj_Type::Enemy_Base);
					Static_Pointer_Object_Arr[i][j] = new_obj;
					Enemy_Base = new_obj;
					break;
					//player base
				case 6:
					new_obj = new AStatic_Object(j, i, EStatic_Obj_Type::Player_Base);
					Static_Pointer_Object_Arr[i][j] = new_obj;
					Player_Base = new_obj;
					break;
				//spauners
				case 7:
					new_spauner = new ASpauner(j, i);
					Spauners.push_back(new_spauner);
					break;
				//player start pos
				case 8:
					Player->Set_Pos(j, i);
					break;
				default:
					break;
				}
			}
		}
	}
}
//===============================================================================================================================
void AsLevel::Check_Collosion_Project_Tile(AParticle* obj)
{
	AItem* new_item{};
	EDirection dir{};
	EStatic_Obj_Type obj_type{};
	sf::Vector2f pos = obj->Get_Position(dir);

	AStatic_Object* cur_obj = nullptr;
	int cx{}, cy{};
	int nx{}, ny{};
	sf::Vector2f cur_obj_pos;
	float distance{};

	int graf_recalculate_pos_x{}, graf_recalculate_pos_y{};


	cx = (int)(pos.x / 50.f);
	cy = (int)(pos.y / 50.f);

	cx = cx < 0 ? 0 : cx;
	cy = cy < 0 ? 0 : cy;
	cx = cx > AsSettings::Level_Width - 1 ? AsSettings::Level_Width - 1 : cx;
	cy = cy > AsSettings::Level_Height - 1 ? AsSettings::Level_Height  - 1: cy;

	cur_obj = Static_Pointer_Object_Arr[cy][cx];
	if (cur_obj != nullptr)
	{
		cur_obj_pos = cur_obj->Get_Pos();
		cur_obj_pos.x += 25.f;
		cur_obj_pos.y += 25.f;

		distance = AsSettings::Distance_Beetween_Two_Points(pos, cur_obj_pos);
		if (distance < 32.f)
		{
			if (!cur_obj->Hit(dir))
			{
				Sound_Object_Destroy.play();

				cur_obj->Get_Start_Pos(cur_obj_pos);
				Add_Exp_Particle(cur_obj_pos, EParticles_Type::Tank_Shoot3);

				//Graf_recalculate
				cur_obj->Get_Ceil_Pos(graf_recalculate_pos_x, graf_recalculate_pos_y);
				Cur_Level_Data.Level_Array[graf_recalculate_pos_y][graf_recalculate_pos_x] = 0;
				Path_Finder.Recalculate_Graf_Segment(graf_recalculate_pos_x, graf_recalculate_pos_y, *Cur_Level_Data.Level_Array);

				//check type
				obj_type = cur_obj->Get_Type();
				switch (obj_type)
				{
				case EStatic_Obj_Type::Enemy_Base:
					*Game_Messages = EGame_Messages::Next_Level;
					break;
				case EStatic_Obj_Type::Player_Base:
					*Game_Messages = EGame_Messages::Restart_Level;
					break;
				case EStatic_Obj_Type::Barrel:
					new_item = new AItem(cur_obj->Get_Pos(), (EItem_Type)(rand() % 3));
					Items.push_back(new_item);
					break;
				default:
					break;
				}

				delete cur_obj;
				Static_Pointer_Object_Arr[cy][cx] = nullptr;
			}
			obj->Delete_Instance();
		}
	}

	return;
}
//===============================================================================================================================
void AsLevel::Check_Hit_Enemy(AParticle* obj)
{
	EDirection dir{};
	sf::Vector2f pos = obj->Get_Position(dir);

	std::vector<AEnemy*>::iterator it;
	int distance{};

	for (it = Enemys.begin(); it != Enemys.end(); ++it)
	{
		distance = AsSettings::Distance_Beetween_Two_Points((*it)->Get_Pos(), pos );

		if (distance < AsSettings::Ceil_Size / 2)
		{
			obj->Delete_Instance();
			(*it)->Hit();
			break;
		}
	}
}
//===============================================================================================================================
void AsLevel::Add_Exp_Particle(sf::Vector2f pos, EParticles_Type type)
{
	AParticle* particle;

	particle = new AParticle(pos, type);
	Particles.push_back(particle);
}
//===============================================================================================================================
void AsLevel::Spauners_Act()
{
	int random_index;
	EEnemys_Type new_enemy_type{};

	Timer += 0.03f * AsSettings::Get_Delta_Time();
	if (Timer > Spaun_Time_Limit)
	{
		Timer = 0.f;

		//choose random spauner
		random_index = rand() % Spauners.size();

		//choose random enemy type
		new_enemy_type = (EEnemys_Type)(rand() % 4);

		switch (new_enemy_type)
		{
		case EEnemys_Type::Type_A:
			if (Type_A_Count < Cur_Level_Data.Limit_Type_A)
			{
				++Type_A_Count;
				Add_Enemys(Spauners.at(random_index), new_enemy_type);
			}
			break;
		case EEnemys_Type::Type_B:
			if (Type_B_Count < Cur_Level_Data.Limit_Type_B)
			{
				++Type_B_Count;
				Add_Enemys(Spauners.at(random_index), new_enemy_type);
			}
			break;
		case EEnemys_Type::Type_C:
			if (Type_C_Count < Cur_Level_Data.Limit_Type_C)
			{
				++Type_C_Count;
				Add_Enemys(Spauners.at(random_index), new_enemy_type);
			}
			break;
		case EEnemys_Type::Type_D:
			if (Type_D_Count < Cur_Level_Data.Limit_Type_D)
			{
				++Type_D_Count;
				Add_Enemys(Spauners.at(random_index), new_enemy_type);
			}
			break;
		default:
			break;
		}
	}
}
//===============================================================================================================================
void AsLevel::Add_Enemys(ASpauner* spauner, EEnemys_Type enemy_type)
{
	float ceil_size = (float)AsSettings::Ceil_Size;
	int spauner_x{}, spauner_y{};
	sf::Vector2f obj_pos{};
	int obj_x{}, obj_y{};
	std::vector<AEnemy*>::iterator it;

	spauner->Get_Pos(spauner_x, spauner_y);

	for (it = Enemys.begin(); it != Enemys.end(); ++it)
	{
		obj_pos = (*it)->Get_Pos();
		obj_x = (int)(obj_pos.x / ceil_size);
		obj_y = (int)(obj_pos.y / ceil_size);
		if (spauner_x == obj_x && spauner_y == obj_y)
			return;
	}

	Enemys.push_back(new AEnemy(spauner_x, spauner_y, enemy_type, this));
}
//===============================================================================================================================
void AsLevel::Pathfinder_Act()
{
	sf::Sprite player_sprite;

	int size = Enemys.size();
	if (size <= 0)
		return;
	else
	{
		AEnemy* enemy_obj;
		bool is_mooving{};

		float ceil_size;
		EDirection dir{};
		sf::Vector2f player_pos{}, enemy_obj_pos{};
		SCoord start{}, goal_a{}, goal_b{}, goal_c{};
		EEnemys_Type cur_type{};
		std::vector<SCoord>* cur_obj_path{};

		if (Path_Finder_Indicator >= size)
			Path_Finder_Indicator = 0;

		enemy_obj = Enemys.at(Path_Finder_Indicator);

		Prepare_To_Shooting(enemy_obj);

		is_mooving = enemy_obj->Get_State()->Is_Mooving_Substate();
		if (is_mooving)
			return;

		ceil_size = (float)AsSettings::Ceil_Size;

		player_sprite = Player->Get_Sprite();

		player_pos = player_sprite.getPosition();

		enemy_obj_pos = enemy_obj->Get_Pos();

		start.X = (int)(enemy_obj_pos.x / ceil_size);
		start.Y = (int)(enemy_obj_pos.y / ceil_size);

		//player pos
		goal_a.X = (int)(player_pos.x / ceil_size);
		goal_a.Y = (int)(player_pos.y / ceil_size);

		//player_base pos
		Player_Base->Get_Ceil_Pos(goal_b.X, goal_b.Y);

		cur_type = enemy_obj->Get_Type();
		cur_obj_path = enemy_obj->Get_Path_Vector();

		switch (cur_type)
		{
		case EEnemys_Type::Type_A:
			Path_Finder.Get_Path(start, goal_a, *(cur_obj_path), true);
			cur_obj_path->pop_back();
			if (cur_obj_path->size() <= 1)
			{
				goal_c = Path_Finder.Get_Right_Free_Ceil(start);
				cur_obj_path->push_back(goal_c);
			}
			break;
		case EEnemys_Type::Type_B:
			Path_Finder.Get_Path(start, goal_a, *(cur_obj_path), true);
			cur_obj_path->pop_back();
			if (cur_obj_path->size() <= 1)
			{
				Path_Finder.Get_Path(start, goal_b, *(cur_obj_path), false);
				cur_obj_path->pop_back();
			}
			break;
		case EEnemys_Type::Type_C:
			Path_Finder.Get_Path(start, goal_b, *(cur_obj_path), true);
			cur_obj_path->pop_back();
			if (cur_obj_path->size() <= 1)
			{
				Path_Finder.Get_Path(start, goal_a, *(cur_obj_path), true);
				cur_obj_path->pop_back();
				if (cur_obj_path->size() <= 1)
				{
					goal_c = Path_Finder.Get_Right_Free_Ceil(start);
					cur_obj_path->push_back(goal_c);
				}
			}
			break;
		case EEnemys_Type::Type_D:
			Path_Finder.Get_Path(start, goal_a, *(cur_obj_path), false);
			cur_obj_path->pop_back();
			break;
		default:
			break;
		}

		++Path_Finder_Indicator;
	}
}
//===============================================================================================================================
void AsLevel::Prepare_To_Shooting(AEnemy* enemy_obj)
{
	bool result{};
	EDirection dir_result{};
	EDirection dir{};
	EEnemy_Target target;
	sf::Vector2i enother_enemys_pos;
	sf::Vector2i target_ceil_pos;
	sf::Vector2i obj_pos;
	sf::Vector2i direction_offset;
	std::vector<AEnemy*>::iterator it;
	AEnemy_State* obj_state = nullptr;


	obj_state = enemy_obj->Get_State();


	if (obj_state->Is_Shooting())
		return;
	/*if ( obj_state->Is_Mooving_Substate() )
		return;*/

	target = enemy_obj->Get_Target();


	obj_pos = (sf::Vector2i)enemy_obj->Get_Pos() / AsSettings::Ceil_Size;

	switch (target)
	{
	case EEnemy_Target::Player:
		target_ceil_pos = (sf::Vector2i)Player->Get_Sprite().getPosition() / AsSettings::Ceil_Size;
		break;
	case EEnemy_Target::Base:
		target_ceil_pos = (sf::Vector2i)Player_Base->Get_Pos() / AsSettings::Ceil_Size;
		break;
	default:
		break;
	}

	if (target_ceil_pos.x != obj_pos.x && target_ceil_pos.y != obj_pos.y)
		return;

	obj_pos = (sf::Vector2i)enemy_obj->Get_Pos(dir) / AsSettings::Ceil_Size;

	switch (dir)
	{
	case EDirection::Top:
		if (target_ceil_pos.x == obj_pos.x && target_ceil_pos.y < obj_pos.y)
		{
			dir_result = EDirection::Top;
			result = true;
		}
		break;
	case EDirection::Right:
		if (target_ceil_pos.y == obj_pos.y && target_ceil_pos.x > obj_pos.x)
		{
			dir_result = EDirection::Right;
			result = true;
		}

		break;
	case EDirection::Bottom:
		if (target_ceil_pos.x == obj_pos.x && target_ceil_pos.y > obj_pos.y)
		{
			dir_result = EDirection::Bottom;
			result = true;
		}
		break;
	case EDirection::Left:
		if (target_ceil_pos.y == obj_pos.y && target_ceil_pos.x < obj_pos.x)
		{
			dir_result = EDirection::Left;
			result = true;
		}
		break;
	default:
		break;
	}

 	for (it = Enemys.begin(); it != Enemys.end(); ++it)
	{
		enother_enemys_pos = (sf::Vector2i)(*it)->Get_Pos() / AsSettings::Ceil_Size;
		if (enother_enemys_pos.x == obj_pos.x || enother_enemys_pos.y == obj_pos.y)
		{
			switch (dir_result)
			{
			case EDirection::Top:
				if (enother_enemys_pos.x == obj_pos.x && enother_enemys_pos.y < obj_pos.y && enother_enemys_pos.y > target_ceil_pos.y)
					result = false;
				break;
			case EDirection::Right:
				if (enother_enemys_pos.y == obj_pos.y && enother_enemys_pos.x > obj_pos.x && enother_enemys_pos.x < target_ceil_pos.x)
					result = false;
				break;
			case EDirection::Bottom:
				if (enother_enemys_pos.x == obj_pos.x && enother_enemys_pos.y > obj_pos.y && enother_enemys_pos.y < target_ceil_pos.y)
					result = false;
				break;
			case EDirection::Left:
				if (enother_enemys_pos.y == obj_pos.y && enother_enemys_pos.x < obj_pos.x && enother_enemys_pos.x > target_ceil_pos.x)
					result = false;
				break;
			default:
				break;
			}
		}
	}

	if (result)
	{
		*(obj_state) = EEnemy_State::Shooting;
		Add_Explosion_Particle(enemy_obj, dir_result);
		Sound_Shoot.play();
	}
}
//===============================================================================================================================
void AsLevel::ProjectTile_Chek_Hit_Player(AParticle* obj)
{
	EDirection dir{};
	int distance{};

	sf::Vector2f pos = obj->Get_Position(dir);

	sf::Vector2f pos_player = Player->Get_Sprite().getPosition();


	distance = AsSettings::Distance_Beetween_Two_Points(pos_player, pos);

	if (distance < AsSettings::Ceil_Size / 2)
	{
		obj->Delete_Instance();
		Player->Hit();
	}
}
//===============================================================================================================================