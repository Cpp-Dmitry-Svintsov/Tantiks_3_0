#include "constructor.h"

//===============================================================================================================================
// constructor & destructor:
//===============================================================================================================================
AsConstructor::~AsConstructor()
{}
AsConstructor::AsConstructor()
	:Selected_Element(0)
{
	Cur_Lvl_Rect.setPosition(sf::Vector2f(0.f, 50.f));
	Cur_Lvl_Rect.setSize(sf::Vector2f((float)AsSettings::Level_Width * 50.f, (float)AsSettings::Level_Height * 50.f));
	Cur_Lvl_Rect.setFillColor(sf::Color::Black);

	//pointer to the selected element
	Pointer_Pos.setPosition(sf::Vector2f(0.f, 0.f));
	Pointer_Pos.setSize(sf::Vector2f(50.f, 50.f));
	Pointer_Pos.setFillColor(sf::Color::Transparent);
	Pointer_Pos.setOutlineThickness(3);
	Pointer_Pos.setOutlineColor(sf::Color::Red);
}
//===============================================================================================================================
// public secton:
//===============================================================================================================================
void AsConstructor::Update(sf::RenderWindow& window)
{
	int lvl_width = AsSettings::Level_Width * 50;
	int button_number{};
	sf::Vector2i element_pos;
	sf::Vector2i ceil_pos;
	if (Selected_Element > 8)
		Selected_Element = 8;
	Pointer_Pos.setPosition(sf::Vector2f((double)Selected_Element * 50.f, 0.f));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		ceil_pos = sf::Mouse::getPosition(window);
		if (ceil_pos.y < 50)
			Selected_Element = ceil_pos.x / 50;
		else if (ceil_pos.y < 50 + lvl_width)
		{
			element_pos.x = ceil_pos.x / 50;
			element_pos.y = (ceil_pos.y - 50) / 50;
			if (Selected_Element == 8)
				Level_Data.Level_Array[element_pos.y][element_pos.x] = 0;
			else
				Level_Data.Level_Array[element_pos.y][element_pos.x] = Selected_Element + 1;
		}
		else
		{
			button_number = (int)ceil_pos.x / 50;
			switch (button_number)
			{
			case 0:
				--Level_Data.Limit_Type_A;
				if (Level_Data.Limit_Type_A < 0)
					Level_Data.Limit_Type_A = 0;
				break;
			case 2:
				++Level_Data.Limit_Type_A;
				if (Level_Data.Limit_Type_A > 15)
					Level_Data.Limit_Type_A = 15;
				break;


			case 3:
				--Level_Data.Limit_Type_B;
				if (Level_Data.Limit_Type_B < 0)
					Level_Data.Limit_Type_B = 0;
				break;
			case 5:
				++Level_Data.Limit_Type_B;
				if (Level_Data.Limit_Type_B > 15)
					Level_Data.Limit_Type_B = 15;
				break;


			case 6:
				--Level_Data.Limit_Type_C;
				if (Level_Data.Limit_Type_C < 0)
					Level_Data.Limit_Type_C = 0;
				break;
			case 8:
				++Level_Data.Limit_Type_C;
				if (Level_Data.Limit_Type_C > 15)
					Level_Data.Limit_Type_C = 15;
				break;

			case 9:
				--Level_Data.Limit_Type_D;
				if (Level_Data.Limit_Type_D < 0)
					Level_Data.Limit_Type_D = 0;
				break;
			case 11:
				++Level_Data.Limit_Type_D;
				if (Level_Data.Limit_Type_D > 15)
					Level_Data.Limit_Type_D = 15;
				break;



			case 14:
				Level_Data.Limit_Type_A = 0;
				Level_Data.Limit_Type_B = 0;
				Level_Data.Limit_Type_C = 0;
				Level_Data.Limit_Type_D = 0;
				memset(Level_Data.Level_Array, 0, sizeof(Level_Data.Level_Array));
				break;

			case 15:
				Save_Data();
				break;

			default:
				break;
			}
		}
	}
}
void AsConstructor::Draw(sf::RenderWindow& window)
{
	int ceil_size = AsSettings::Ceil_Size;

	Draw_Work_Space(window);

	Draw_Objects(window);

	Draw_Info_Pannel(window);

	window.draw(Pointer_Pos);
}
//===============================================================================================================================
// private section:
//===============================================================================================================================
void AsConstructor::Draw_Work_Space(sf::RenderWindow& window)
{
	int i{}, j{};
	int ceil_size = AsSettings::Ceil_Size;
	int lvl_width = AsSettings::Level_Width;
	int lvl_heigt = AsSettings::Level_Height;

	window.draw(Cur_Lvl_Rect);

	for (i = 1; i < lvl_heigt + 2; ++i)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(0.f, (float)(i * ceil_size))),
			sf::Vertex(sf::Vector2f(lvl_width * 50, (float)(i * ceil_size)))
		};

		window.draw(line, 2, sf::Lines);
	}

	for (j = 0; j < lvl_width + 1; ++j)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f((float)(j * ceil_size), 50.f)),
			sf::Vertex(sf::Vector2f((float)(j * ceil_size), (lvl_heigt + 1) * 50))
		};

		window.draw(line, 2, sf::Lines);
	}
}
//===============================================================================================================================
void AsConstructor::Draw_Objects(sf::RenderWindow& window)
{

	sf::Texture static_texture;
	sf::Sprite sprite;
	static_texture.loadFromFile("textures/static_obj.png");
	sprite.setTexture(static_texture);

	int ceil_size = AsSettings::Ceil_Size;

	int i{}, j{};
	int lvl_width = AsSettings::Level_Width;
	int lvl_heigt = AsSettings::Level_Height;
	int elem{};

	window.draw(sprite);

	for (i = 0; i < lvl_width; ++i)
	{
		for (j = 0; j < lvl_heigt; ++j)
		{
			elem = Level_Data.Level_Array[i][j];
			if (elem != 0)
			{
				--elem;
				sprite.setTextureRect(sf::IntRect(elem * 50, 0, 50, 50));
				sprite.setPosition(sf::Vector2f(j * 50.f, i * 50.f + 50.f));
				window.draw(sprite);
			}
		}
	}
}
//===============================================================================================================================
void AsConstructor::Draw_Info_Pannel(sf::RenderWindow& window)
{
	int number{};
	int ceil_size = AsSettings::Ceil_Size;
	float f_ceil_size = (float)ceil_size;
	float y_offset = (float)(AsSettings::Level_Height * ceil_size + ceil_size);

	std::stringstream std_str;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Font font;
	sf::Text text;
	texture.loadFromFile("textures/constructor_icon.png");
	font.loadFromFile("fonts/Jersey.ttf");
	sprite.setTexture(texture);
	text.setFont(font);
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::White);


	//Limit Type A
	sprite.setPosition(sf::Vector2f(0.f, y_offset));
	sprite.setTextureRect(sf::IntRect(0, 0, ceil_size, ceil_size));
	window.draw(sprite);
	text.setPosition(sf::Vector2f(f_ceil_size, y_offset - 10.f));
	std_str.str("");
	number = Level_Data.Limit_Type_A;
	std_str << number;
	text.setString(std_str.str());
	window.draw(text);
	sprite.setPosition(sf::Vector2f(100.f, y_offset));
	sprite.setTextureRect(sf::IntRect(ceil_size, 0, ceil_size, ceil_size));
	window.draw(sprite);

	//Limit Type B
	sprite.setPosition(sf::Vector2f(150.f, y_offset));
	sprite.setTextureRect(sf::IntRect(0, 0, ceil_size, ceil_size));
	window.draw(sprite);
	text.setPosition(sf::Vector2f(f_ceil_size * 4, y_offset - 10.f));
	std_str.str("");
	number = Level_Data.Limit_Type_B;
	std_str << number;
	text.setString(std_str.str());
	window.draw(text);
	sprite.setPosition(sf::Vector2f(250.f, y_offset));
	sprite.setTextureRect(sf::IntRect(ceil_size, 0, ceil_size, ceil_size));
	window.draw(sprite);

	//Limit Type C
	sprite.setPosition(sf::Vector2f(300.f, y_offset));
	sprite.setTextureRect(sf::IntRect(0, 0, ceil_size, ceil_size));
	window.draw(sprite);
	text.setPosition(sf::Vector2f(f_ceil_size * 7, y_offset - 10.f));
	std_str.str("");
	number = Level_Data.Limit_Type_C;
	std_str << number;
	text.setString(std_str.str());
	window.draw(text);
	sprite.setPosition(sf::Vector2f(400.f, y_offset));
	sprite.setTextureRect(sf::IntRect(ceil_size, 0, ceil_size, ceil_size));
	window.draw(sprite);

	//Limit Type D
	sprite.setPosition(sf::Vector2f(450.f, y_offset));
	sprite.setTextureRect(sf::IntRect(0, 0, ceil_size, ceil_size));
	window.draw(sprite);
	text.setPosition(sf::Vector2f(f_ceil_size * 10, y_offset - 10.f));
	std_str.str("");
	number = Level_Data.Limit_Type_D;
	std_str << number;
	text.setString(std_str.str());
	window.draw(text);
	sprite.setPosition(sf::Vector2f(550.f, y_offset));
	sprite.setTextureRect(sf::IntRect(ceil_size, 0, ceil_size, ceil_size));
	window.draw(sprite);



	//reset all data
	sprite.setPosition(sf::Vector2f(700.f, y_offset));
	sprite.setTextureRect(sf::IntRect(ceil_size * 2, 0, ceil_size, ceil_size));
	window.draw(sprite);

	//save all data
	sprite.setPosition(sf::Vector2f(750.f, y_offset));
	sprite.setTextureRect(sf::IntRect(ceil_size * 3, 0, ceil_size, ceil_size));
	window.draw(sprite);

}
//===============================================================================================================================
void AsConstructor::Save_Data()
{
	std::ofstream outfile;

	outfile.open("levels/level_test.dat", std::ios::binary | std::ios::out);
	outfile.write(reinterpret_cast<char*>(&Level_Data), sizeof(Level_Data));
	outfile.close();
}
//===============================================================================================================================