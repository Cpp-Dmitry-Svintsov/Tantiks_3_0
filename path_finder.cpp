#include "path_finder.h"


//===============================================================================================================================
// constructor & destructor & init
//===============================================================================================================================
APath_Finder::~APath_Finder()
{
	Clear_Data();
}
//===============================================================================================================================
APath_Finder::APath_Finder()
{
}
//===============================================================================================================================
void APath_Finder::Init()
{
	//top
	Ways[0].X = 0;
	Ways[0].Y = -1;
	//right
	Ways[0].X = 1;
	Ways[0].Y = 0;
	//bottom
	Ways[0].X = 0;
	Ways[0].Y = 1;
	//left
	Ways[0].X = -1;
	Ways[0].Y = 0;
}
//===============================================================================================================================
// public section:
//===============================================================================================================================
void APath_Finder::Get_Static_Graf( unsigned char* lvl_data )
{
	unsigned char i{}, j{};
	int lvl_width{}, lvl_height{};
	int index{};

	lvl_width = AsSettings::Level_Width;
	lvl_height = AsSettings::Level_Height;

	for (i = 0; i < lvl_height; ++i)
	{
		for (j = 0; j < lvl_width; ++j)
		{

			if (*(lvl_data + index) == 0 || *(lvl_data + index) == 3 || *(lvl_data + index) == 8 || *(lvl_data + index) == 7)
			{
				Get_Nearly_Coordinates(j, i, lvl_data, &Graf_Static[i][j] );
			}
			++index;
		}
	}
}
//===============================================================================================================================
void APath_Finder::Get_Clear_Graf(unsigned char* lvl_data)
{
	{
		unsigned char i{}, j{};
		int lvl_width{}, lvl_height{};
		int index{};

		lvl_width = AsSettings::Level_Width;
		lvl_height = AsSettings::Level_Height;

		for (i = 0; i < lvl_height; ++i)
		{
			for (j = 0; j < lvl_width; ++j)
			{
				if (*(lvl_data + index) == 2)
				{
					++index;
					continue;
				}

				Get_Nearly_Coordinates_for_Clear_Craph(j, i, lvl_data, &Graf_Clear[i][j]);
				++index;
			}
		}
	}
};
//===============================================================================================================================
void APath_Finder::Print()
{
	unsigned char len{};
	int i{}, j{}, k{};
	int lvl_width{}, lvl_height{};
	std::stringstream my_str;

	lvl_width = AsSettings::Level_Width;
	lvl_height = AsSettings::Level_Height;

	my_str.str(" ");

	my_str << "=============================================================================================================" << "\n";

	for (i = 0; i < lvl_height; ++i)
	{
		for (j = 0; j < lvl_width; ++j)
		{
			len = Graf_Clear[i][j].Len;
			if (len > 0)
			{
				my_str << "( " << j << "," << i << " )" << " : ";
				for (k = 0; k < len; ++k)
					my_str << Graf_Clear[i][j].Pos[k].X << "," << Graf_Clear[i][j].Pos[k].Y << "; ";
				my_str << "\n";
			}
		}
	}

	my_str << "=============================================================================================================" << "\n";

	printf(my_str.str().c_str());
}
//===============================================================================================================================
void APath_Finder::Get_Path(SCoord start_coord, SCoord goal_coord, std::vector<SCoord> &path, bool is_static_graph)
{
	std::map<SCoord, SCoord> visited;
	std::map<SCoord, SCoord>::iterator it;
	//std::vector<SCoord> path;

	SCoord step;

	path.erase(path.begin(), path.end());


	Dejkstra_A_Starr(start_coord, goal_coord, visited, is_static_graph);

	path.push_back(goal_coord);
	it = visited.find(goal_coord);
	if (it != visited.end())
		step = it->second;
	else
		step = start_coord;

	while (step != start_coord)
	{
		path.push_back(step);
		it = visited.find(step);
		if (it != visited.end())
			step = it->second;
		else
			step = start_coord;
	}

	path.push_back(start_coord);

	//for debaging print result
	/*std::vector<SCoord>::iterator it2;
	std::stringstream my_str;

	my_str.str(" ");

	for (it2 = path.begin(); it2 != path.end(); ++it2)
	{
		my_str << it2->X << " : " << it2->Y << "\n";
	}

	my_str << "--------------------------------------------" << "\n";

	printf(my_str.str().c_str());*/

}
//===============================================================================================================================
SCoord APath_Finder::Get_Right_Free_Ceil(SCoord start)
{
	SCoord goal{};
	SNode_Elem *graf_elem;
	int node_elem_len{};
	int node_elem_pos_index{};


	graf_elem = &Graf_Static[start.Y][start.X];
	node_elem_len = graf_elem->Len;

	node_elem_pos_index = rand() % node_elem_len;

	goal = graf_elem->Pos[node_elem_pos_index];

	return goal;
}
//===============================================================================================================================
void APath_Finder::Recalculate_Graf_Segment(int x, int y, unsigned char* lvl_data)
{
	int i{}, j{};
	int index{};
	sf::Vector2i start_ind;
	sf::Vector2i end_ind;

	start_ind.x = x > 0 ? x - 1 : x;
	start_ind.y = y > 0 ? y - 1 : y;

	end_ind.x = x < AsSettings::Level_Width - 1 ? x + 1 : x;
	end_ind.y = y < AsSettings::Level_Height - 1 ? y + 1 : y;

	for ( i = start_ind.y; i <= end_ind.y; ++ i)
		for (j = start_ind.x; j <= end_ind.x; ++j)
		{
			index = i * AsSettings::Level_Width + j;
			Graf_Static[i][j].Len = 0;

			if (*(lvl_data + index) == 0 || *(lvl_data + index) == 3 || *(lvl_data + index) == 8 || *(lvl_data + index) == 7)
			{
				Get_Nearly_Coordinates(j, i, lvl_data, &Graf_Static[i][j]);
			}
		}
}
//===============================================================================================================================
// private section:
//===============================================================================================================================
void APath_Finder::Clear_Data()
{
	unsigned char len{};
	int i{}, j{};
	int lvl_width{}, lvl_height{};

	lvl_width = AsSettings::Level_Width;
	lvl_height = AsSettings::Level_Height;

	for (i = 0; i < lvl_height; ++i)
		for (j = 0; j < lvl_width; ++j)
		{
			Graf_Static[i][j].Len = 0;
			Graf_Clear[i][j].Len = 0;
		}
}
//===============================================================================================================================
void APath_Finder::Get_Nearly_Coordinates(unsigned char x, unsigned char y, unsigned char* lvl_data, SNode_Elem* node_elem)
{
	int index{};
	EStatic_Obj_Type obj_type{};
	SCoord cord;

	//top ceil
	if (y > 0)
	{
		index = (y - 1) * AsSettings::Level_Width + x;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type == EStatic_Obj_Type::None || obj_type == EStatic_Obj_Type::Enemy_Spauner ||
			obj_type == EStatic_Obj_Type::Player_Spauner || obj_type == EStatic_Obj_Type::Forest)
		{
			cord.X = x;
			cord.Y = y - 1;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//right ceil
	if (x < AsSettings::Level_Width - 1)
	{
		index = y * AsSettings::Level_Width + x + 1;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type == EStatic_Obj_Type::None || obj_type == EStatic_Obj_Type::Enemy_Spauner ||
			obj_type == EStatic_Obj_Type::Player_Spauner || obj_type == EStatic_Obj_Type::Forest)
		{
			cord.X = x + 1;
			cord.Y = y;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//bottom ceil
	if (y < AsSettings::Level_Height - 1)
	{
		index = (y + 1) * AsSettings::Level_Width + x;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type == EStatic_Obj_Type::None || obj_type == EStatic_Obj_Type::Enemy_Spauner ||
			obj_type == EStatic_Obj_Type::Player_Spauner || obj_type == EStatic_Obj_Type::Forest)
		{
			cord.X = x;
			cord.Y = y + 1;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//left ceil
	if (x > 0)
	{
		index = y * AsSettings::Level_Width + x - 1;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type == EStatic_Obj_Type::None || obj_type == EStatic_Obj_Type::Enemy_Spauner ||
			obj_type == EStatic_Obj_Type::Player_Spauner || obj_type == EStatic_Obj_Type::Forest)
		{
			cord.X = x - 1;
			cord.Y = y;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}
}
//===============================================================================================================================
void APath_Finder::Get_Nearly_Coordinates_for_Clear_Craph(unsigned char x, unsigned char y, unsigned char* lvl_data, SNode_Elem* node_elem)
{
	int index{};
	EStatic_Obj_Type obj_type{};
	SCoord cord;

	//top ceil
	if (y > 0)
	{
		index = (y - 1) * AsSettings::Level_Width + x;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type != EStatic_Obj_Type::Wall )
		{
			cord.X = x;
			cord.Y = y - 1;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//right ceil
	if (x < AsSettings::Level_Width - 1)
	{
		index = y * AsSettings::Level_Width + x + 1;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type != EStatic_Obj_Type::Wall)
		{
			cord.X = x + 1;
			cord.Y = y;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//bottom ceil
	if (y < AsSettings::Level_Height - 1)
	{
		index = (y + 1) * AsSettings::Level_Width + x;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type != EStatic_Obj_Type::Wall)
		{
			cord.X = x;
			cord.Y = y + 1;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}

	//left ceil
	if (x > 0)
	{
		index = y * AsSettings::Level_Width + x - 1;
		obj_type = (EStatic_Obj_Type) * (lvl_data + index);
		if (obj_type != EStatic_Obj_Type::Wall)
		{
			cord.X = x - 1;
			cord.Y = y;
			*(node_elem->Pos + node_elem->Len) = cord;
			++node_elem->Len;
		}
	}
}
//===============================================================================================================================
int APath_Finder::Get_Heuristick(SCoord a, SCoord b)
{
	return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
	//return abs((a.X - b.X) + (a.Y - b.Y));
}
//===============================================================================================================================
void APath_Finder::Dejkstra_A_Starr(SCoord start, SCoord goal, std::map<SCoord, SCoord>& visited, bool is_static_graph )
{
	int k{}, v{};
	int len{};
	int i{};
	int cur_cost{};
	int new_cost{};
	int priority{};

	std::map<SCoord, int> queue_heap;
	std::map<SCoord, int>::iterator queue_itr, queue_itr_min;

	std::map<SCoord, int> cost_visited;

	SCoord cur_node;
	SCoord neight_node;

	queue_heap[start] = 0;

	cost_visited[start] = 0;
	visited[start] = start;

	int test{};
	int a{};

	while (queue_heap.size() > 0)
	{
		++test;
		queue_itr_min = queue_heap.begin();
		for (queue_itr = queue_heap.begin(); queue_itr != queue_heap.end(); ++queue_itr)
			if (queue_itr->second < queue_itr_min->second)
				queue_itr_min = queue_itr;

		cur_node = queue_itr_min->first;
		cur_cost = queue_itr_min->second;
		queue_heap.erase(queue_itr_min);

		if (cur_node == goal)
			break;

		k = cur_node.X;
		v = cur_node.Y;

		if (is_static_graph)
			len = Graf_Static[v][k].Len;
		else
			len = Graf_Clear[v][k].Len;

		if (len > 0)
		{
			for (i = 0; i < len; ++i)
			{
				if (is_static_graph)
					neight_node = Graf_Static[v][k].Pos[i];
				else
					neight_node = Graf_Clear[v][k].Pos[i];
				new_cost = cost_visited[cur_node] + 1;
				queue_itr = cost_visited.find(neight_node);
				if (queue_itr != cost_visited.end())
				{
					if (new_cost < queue_itr->second)
					{
						priority = new_cost + Get_Heuristick(neight_node, goal);
						queue_heap[neight_node] = priority;
						cost_visited[neight_node] = new_cost;
						visited[neight_node] = cur_node;
					}
				}
				else
				{
					priority = new_cost + Get_Heuristick(neight_node, goal);
					queue_heap[neight_node] = priority;
					//cost_visited[neight_node] = priority;
					cost_visited[neight_node] = new_cost;
					visited[neight_node] = cur_node;
				}
			}
		}
	}

	//!!!!!!!!!!!!!!!!!!!!for debaginng!!!!!!!!!!!!!
	/*std::stringstream my_str;
	int count{};
	my_str.str();
	my_str << "-----------------------------------------------" << "\n";
	my_str << test << "\n";
	my_str << "-----------------------------------------------" << "\n";
	std::map<SCoord, SCoord>::iterator it2;

	my_str << "------------  visited  -----------------" << "\n";
	for (it2 = visited.begin(); it2 != visited.end(); ++it2)
	{
		++count;
		my_str<< count << ".  " << it2->first.X << " : " << it2->first.Y << " -> " << it2->second.X << " : " << it2->second.Y << "\n";
	}

	my_str << "-----------------------------------------------" << "\n";

	std::map<SCoord, int>::iterator it3;

	count = 0;
	my_str << "------------  visited price  -----------------" << "\n";
	for (it3 = cost_visited.begin(); it3 != cost_visited.end(); ++it3)
	{
		++count;
		my_str << count << ".  " << it3->first.X << " : " << it3->first.Y << " :-: " << it3->second << "\n";
	}

	my_str << "-----------------------------------------------" << "\n";

	printf(my_str.str().c_str());*/

}
//===============================================================================================================================
