#pragma once
#include "level.h"
#include "game_state.h"
#include "info_pannel.h"
#include "constructor.h"
#include "menu.h"

class AsEngine : public AEngine_Interface_For_Info_Pannel
{
public:
	~AsEngine();
	AsEngine();

	void Init();
	void Run();

	virtual void Get_Info(int& player_hp, int& cur_lvl_number, int& player_base_hp, int& enemy_base_hp, int& lives);

private:
	void Messeger_Processing();
	void Update_Info_Pannel();

	int Lives;

	sf::RenderWindow* Window;
	AsPlayer Player;
	AsLevel Level;
	AsMenu Menu;
	AsGame_State Game_State;
	AsConstructor Level_Constructor;
	AsInfo_Pannel Info_Pannel;

	EGame_Messages Game_Messages;


	sf::Music Main_Theme;

	sf::SoundBuffer Win_S;
	sf::Sound Sound_Win;

	sf::SoundBuffer Loose_S;
	sf::Sound Sound_Loose;
};