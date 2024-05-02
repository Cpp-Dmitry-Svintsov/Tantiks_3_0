#include "items.h"

//=====================================================================================================================
// 
//=====================================================================================================================
int AItem::Count_Of_Items = 0;
sf::Texture *AItem::Texture = nullptr;

//=====================================================================================================================
// 
//=====================================================================================================================
AItem::~AItem()
{
	--AItem::Count_Of_Items;
	if (AItem::Count_Of_Items <= 0)
	{
		delete AItem::Texture;
		AItem::Texture = nullptr;
	}
}
AItem::AItem(sf::Vector2f pos, EItem_Type type)
	:Item_Type(type), Is_Alive(true)
{
	++AItem::Count_Of_Items;

	if (AItem::Texture == nullptr)
	{
		AItem::Texture = new sf::Texture();
		Texture->loadFromFile("textures/items.png");
	}
	Sprite.setTexture(*Texture);
	Sprite.setTextureRect(sf::IntRect( (int)(type) * AsSettings::Ceil_Size, 0, AsSettings::Ceil_Size, AsSettings::Ceil_Size));
	Sprite.setPosition(pos);

	Pos = (sf::Vector2i)(pos / (float)AsSettings::Ceil_Size);
}

//=====================================================================================================================
// 
//=====================================================================================================================
void AItem::Update()
{
	++Time_To_Live;
	if (Time_To_Live > Time_Tol_Live_Limit)
		Is_Alive = false;
}
//=====================================================================================================================
void AItem::Draw(sf::RenderWindow& window)
{
	window.draw(Sprite);
}
//=====================================================================================================================
void AItem::Take_Item(EItem_Type& type)
{
	type = Item_Type;
	Is_Alive = false;
}
//=====================================================================================================================
bool AItem::Is_Dead()
{
	if (Is_Alive)
		return false;
	else
		return true;
}
//=====================================================================================================================
sf::Vector2i AItem::Get_Pos()
{
	return Pos;
}
//=====================================================================================================================