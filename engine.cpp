#include "engine.h"

//=====================================================================================================================
//=====================================================================================================================
AsEngine::~AsEngine()
{
	if (Window != nullptr)
		delete Window;
}
//=====================================================================================================================
AsEngine::AsEngine()
    :Game_Messages(EGame_Messages::None), Lives(1)
{}
//=====================================================================================================================
void AsEngine::Init()
{
	Window = new sf::RenderWindow(sf::VideoMode(AsSettings::Screen_Width, AsSettings::Screen_Height), L"Tank Game v 3.0", sf::Style::Default);
	Window->setFramerateLimit(AsSettings::Get_Fps());

    Menu.Init(&Game_Messages);

    Level.Init(&Player, &Game_Messages);

    Player.Init(&Level, &Game_Messages);

    Game_State.Init();
    Game_State = EGame_State::Menu;

    Info_Pannel.Init(this);

    //sound
    Main_Theme.openFromFile("sounds/main_theme.mp3");
    Main_Theme.play();
    Main_Theme.setLoop(true);
    Main_Theme.setVolume(50);

    Win_S.loadFromFile("sounds/win.mp3");
    Sound_Win.setBuffer(Win_S);

    Loose_S.loadFromFile("sounds/loose.mp3");
    Sound_Loose.setBuffer(Loose_S);

}
//=====================================================================================================================
void AsEngine::Run()
{
    bool is_key_released;
    float dt{};
    sf::Clock clock;

    AsSettings::Init(&dt);

    while (Window->isOpen())
    {
        is_key_released = false;

        sf::Event event;
        while (Window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window->close();

            if (event.type == sf::Event::KeyReleased)
                is_key_released = true;

        }
        //calculate delta time
        dt = clock.restart().asSeconds();

        //clear the window
        Window->clear(sf::Color::Black);

        switch (Game_State)
        {
        case EGame_State::Menu:
            Menu.Update(is_key_released);
            Menu.Draw(*Window);
            break;
        case EGame_State::Play_Current_Level:
            Game_State.Update();
            Level.Update();
            Player.Update();

            Player.Draw(*Window);
            Level.Draw(*Window);
            Game_State.Draw(*Window);

            Update_Info_Pannel();
            break;
        case EGame_State::Constructor:
            Level_Constructor.Update(*Window);
            Level_Constructor.Draw(*Window);
            break;
        default:
            break;
        }

        //render the window
        Window->display();

        Messeger_Processing();
    }
}
//virtual func:
//=====================================================================================================================
void AsEngine::Get_Info(int& player_hp, int& cur_lvl_number, int& player_base_hp, int& enemy_base_hp, int& lives)
{
    player_hp = Player.Get_Hp();
    Level.Get_Current_Level_Data_Info(cur_lvl_number, player_base_hp, enemy_base_hp);
    lives = Lives;
}
//=====================================================================================================================
// private section:
//=====================================================================================================================
void AsEngine::Messeger_Processing()
{
    //system keydown esc
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        if (Game_State == EGame_State::Play_Current_Level || Game_State == EGame_State::Constructor)
            Game_Messages = EGame_Messages::Return_To_Menu;
        else if (Game_State == EGame_State::Menu)
            Game_Messages = EGame_Messages::Exit;
    }


    switch (Game_Messages)
    {
    case EGame_Messages::None:
        break;
    case EGame_Messages::Exit:
        Window->close();
        break;
    case EGame_Messages::Return_To_Menu:
        Game_State = EGame_State::Menu;
        break;
    case EGame_Messages::New_Game:
        Lives = 3;
        Game_State = EGame_State::Play_Current_Level;
        Level.Init_new_level(1);
        break;
    case EGame_Messages::Constructor:
        Game_State = EGame_State::Constructor;
        break;
    case EGame_Messages::Restart_Level:
        --Lives;
        if (Lives > 0)
        {
            Sound_Loose.play();
            Level.Restart_Level();
            Game_State = EGame_State::Play_Current_Level;
        }
        else
            Game_State = EGame_State::Menu;
        break;
    case EGame_Messages::Next_Level:
        if (!Level.Next_Level())
        {
            Game_State = EGame_State::Menu;
        }
        else
        {
            Game_State = EGame_State::Play_Current_Level;
            Sound_Win.play();
        }
        break;
    default:
        break;
    }
    Game_Messages = EGame_Messages::None;
}
//=====================================================================================================================
void AsEngine::Update_Info_Pannel()
{
    Info_Pannel.Update();
    Info_Pannel.Draw(*Window);
}
//=====================================================================================================================