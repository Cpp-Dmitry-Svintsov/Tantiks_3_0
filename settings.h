#pragma once
//sfml
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
//std
#include <sstream>
#include <fstream>
//containers
#include <vector>
#include <string>
#include <map>
#include <queue>


class AsSettings
{
protected:
	~AsSettings();
	AsSettings();

public:
	AsSettings(AsSettings& other) = delete;
	void operator=(AsSettings&) = delete;

	static void Init(float* dt);
	static float Distance_Beetween_Two_Points(sf::Vector2f a, sf::Vector2f b);

	static float Get_Delta_Time();
	static float Get_Fps();

	static const int Ceil_Size = 50;
	static const int Level_Width = 16;
	static const int Level_Height = 16;
	static const int Screen_Width = 800;
	static const int Screen_Height = 900;
	static const int Max_Level_Count = 10;
private:
	static float Fps;
	static float *Delta_Time;
};