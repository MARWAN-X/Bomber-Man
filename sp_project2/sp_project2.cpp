#include"pch.h"
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<functional>
#include<ctime>
#include<time.h>
using namespace sf;
using namespace std;
struct leftmovement {
	int i, j;
};
struct rightmovement {
	int i, j;
};
struct upmovement {
	int i, j;
};
struct downmovement {
	int i, j;
};
struct BFS_Movement {
	int i, j;
}Smart_Skeleton_Movement,Red_Robot_Movement;
//General stuff
const int length = 12, width = 17;
float player_position = 0;
const int player_speed = 2;
float Smart_Monsters_Speed = 0.3;
int Random_Monsters_Speed = 1;

//Skeleton stuff
int Skeleton_Rand_motion = rand() % 4 + 1;
int Skeleton_counter = 0;
float Skeleton_position = 0;

//Robot stuff
int Robot_Rand_motion = rand() % 4 + 1;
int Robot_counter = 0;
float Robot_position = 0;

//Hetlar stuff
int White_Robot_Rand_motion = rand() % 4 + 1;
int White_Robot_counter = 0;
float White_Robot_position = 0;

//Smart_Skeleton stuff
float Smart_Skeleton_position = 0;

//Red_robot stuff
float Red_Robot_position = 0;
float Red_Robot_Speed = 0.6;


// Menu stuff
#define MAX_NUMBER_OF_ITEMS 4
int selectedItemIndex = 0;
Font font; Text menu[MAX_NUMBER_OF_ITEMS];
void setmenustatus(float width, float height) {
	if (!font.loadFromFile("Chocolate Bar Demo.otf"))
	{
		// handle error
		cout << "SHIIIIT" << endl;
	}

	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		menu[i].setFont(font);
		if (i == 0) {
			menu[i].setFillColor(Color::Red);
			menu[i].setString("Single player");
			menu[i].setPosition(sf::Vector2f(width / 2.25, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.35));
		}
		else if (i == 1) {
			menu[i].setFillColor(Color::White);
			menu[i].setString("Multiplayer");
			menu[i].setPosition(sf::Vector2f(width / 2.5, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.1));

		}
		else if (i == 2)
		{
			menu[i].setFillColor(Color::White);
			menu[i].setString("About us");
			menu[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.75));
		}
		else {
			menu[3].setFillColor(Color::White);
			menu[3].setString("Exit");
			menu[3].setPosition(sf::Vector2f(width / 2.25, height / (MAX_NUMBER_OF_ITEMS + 1) * 4.5));
		}

	}
}

// Levels_Menu stuff
#define MAX_NUMBER_OF_levels 3
int selectedlevelIndex = 0;
Font levelsfont; Text levels[MAX_NUMBER_OF_levels];
void setlevelsstatus(float width, float height) {
	if (!levelsfont.loadFromFile("Chocolate Bar Demo.otf"))
	{
		// handle error
		cout << "SHIIIIT" << endl;
	}

	for (int i = 0; i < MAX_NUMBER_OF_levels; i++)
	{
		levels[i].setFont(levelsfont);
		if (i == 0) {
			levels[i].setFillColor(Color::Red);
			levels[i].setString("Level 1");
			levels[i].setPosition(sf::Vector2f(width / 2.25, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.35));
		}
		else if (i == 1) {
			levels[i].setFillColor(Color::White);
			levels[i].setString("Level 2");
			levels[i].setPosition(sf::Vector2f(width / 2.5, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.1));

		}
		else if (i == 2) {

			levels[i].setFillColor(Color::White);
			levels[i].setString("Level 3");
			levels[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.75));

		}
	}
}
void drawmenu(RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);

	}
}
void drawlev(RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_levels; i++)
	{
		window.draw(levels[i]);

	}
}
void MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}
void MoveUplev()
{
	if (selectedlevelIndex - 1 >= 0)
	{
		levels[selectedlevelIndex].setFillColor(Color::White);
		selectedlevelIndex--;
		levels[selectedlevelIndex].setFillColor(Color::Red);
	}
}
void MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}
void MoveDownlev()
{
	if (selectedlevelIndex + 1 < MAX_NUMBER_OF_levels)
	{
		levels[selectedlevelIndex].setFillColor(Color::White);
		selectedlevelIndex++;
		levels[selectedlevelIndex].setFillColor(Color::Red);
	}
}
int GetPressedItem() {
	return selectedItemIndex;
}
int GetPressedlevel() {
	return selectedlevelIndex;
}

int GetXindx(float a)
{
	return (a - 183) / 53 + 2;
}
int GetYindx(float a)
{
	return (a - 74) / 48 + 2;
}
int GetXpostion(int a)
{
	return ((a - 2) * 53) + 198;
}
int GetYpostion(int a)
{
	return ((a - 2) * 48) + 89;
}

bool vis[20][20];
int dx[] = { 0,  0, 1, -1 };
int dy[] = { 1, -1, 0,  0 };
bool valid(int i, int j, Sprite arr[][length + 1], Texture &g1, Texture &g2)
{
	return i >= 2 and j >= 2 and i < width + 1 and j < length + 1 and !vis[i][j] and (arr[i][j].getTexture() == &g1 || arr[i][j].getTexture() == &g2);
}
void BFS(int srcX, int srcY, int distX, int distY, Sprite arr[][length + 1], Texture &g1, Texture &g2)
{
	memset(vis, 0, sizeof vis);
	queue <pair<int, int> > Q;
	vis[srcX][srcY] = 1;
	Q.push({ srcX, srcY });
	while (!Q.empty())
	{
		int nodeX = Q.front().first;
		int nodeY = Q.front().second;
		//cout << nodeY << endl;
		if ((nodeX == distX && nodeY + 1 == distY)) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}
		else if (nodeX == distX && nodeY - 1 == distY) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}
		else if (nodeX + 1 == distX && nodeY == distY) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}
		else if (nodeX - 1 == distX && nodeY == distY) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}

		Q.pop();
		for (int k = 0; k < 4; k++)
		{
			int newNodeX = nodeX + dx[k];
			int newNodeY = nodeY + dy[k];
			if (valid(newNodeX, newNodeY, arr, g1, g2))
			{
				vis[newNodeX][newNodeY] = 1;
				Q.push({ newNodeX, newNodeY });
			}
		}
	}
}

void Generate_Charcter(RectangleShape &charcter, Texture &charcter_texture, Vector2u &charcter_size, string filename, int X_Postion, int Y_Postion) {
	X_Postion = GetXpostion(X_Postion); Y_Postion = GetYpostion(Y_Postion);
	charcter.setPosition(X_Postion, Y_Postion);
	charcter.setScale(0.8, 0.8);
	charcter_texture.loadFromFile(filename);
	charcter.setTexture(&charcter_texture);
	charcter_size = charcter_texture.getSize();
	charcter_size.x /= 9;
	charcter_size.y /= 4;
	charcter.setTextureRect(IntRect(charcter_size.x * 0, charcter_size.y * 2, charcter_size.x, charcter_size.y));
}

void Beginner_Mode() {
	//###############################################################################
	// Making of Player
	RectangleShape player(Vector2f(100, 120));
	Texture player_texture;
	Vector2u player_size;
	Clock player_clock;
	Time player_time;
	Generate_Charcter(player, player_texture, player_size, "mainplayer.png", 10, 7);
	//###############################################################################

	//###############################################################################
	// Making of Yellow_Robot
	RectangleShape Robot(Vector2f(100, 120));
	Texture Yellow_Robot_texture;
	Vector2u Yellow_Robot_size;
	Clock Robot_clock;
	Time Robot_time;
	Generate_Charcter(Robot, Yellow_Robot_texture, Yellow_Robot_size, "yellow_robot.png", 15, 2);
	//###############################################################################

	//###############################################################################
	// Making of Skeleton
	RectangleShape Skeleton(Vector2f(100, 120));
	Texture Skeleton_texture;
	Vector2u Skeleton_size;
	Clock Skeleton_clock;
	Time Skeleton_time;
	Generate_Charcter(Skeleton, Skeleton_texture, Skeleton_size, "monster (5).png", 7, 8);
	//###############################################################################


	//###############################################################################
	// Making of White_Robot
	RectangleShape White_Robot(Vector2f(100, 120));
	Texture White_Robot_texture;
	Vector2u White_Robot_size;
	Clock White_Robot_clock;
	Time White_Robot_time;
	Generate_Charcter(White_Robot, White_Robot_texture, White_Robot_size, "white_robot.png", 14, 8);
	//###############################################################################

	//###############################################################################
	// Movement stuff
	leftmovement left; rightmovement right; upmovement up; downmovement down;
	int Player_xindx = 0, Player_yindx = 0, White_Robot_xindx = 0, White_Robot_yindx = 0, Skeleton_xindx = 0, Skeleton_yindx = 0, Robot_xindx = 0, Robot_yindx = 0;
	//###############################################################################


	// Making of Grid
	Texture wall, ground, wallpaper, ground2, breakable, life, bomb, range, hurt, one, two, three, choice;
	if (!breakable.loadFromFile("breakable.png"))
	{
		cout << "error loading breakable wall pic";
	}
	if (!wallpaper.loadFromFile("wallpaper.jpg"))
	{
		cout << "error loading wallpaper pic";
	}
	if (!wall.loadFromFile("3d wall.png"))
	{
		cout << "error loading wall pic";
	}

	if (!ground.loadFromFile("ground 1.png"))
	{
		cout << "error loading ground pic";
	}
	if (!ground2.loadFromFile("grass.png"))
	{
		cout << "error loading grass pic";
	}

	if (!life.loadFromFile("flife.png"))
	{
		cout << "error loading life pic";
	}
	if (!range.loadFromFile("frange.png"))
	{

		cout << "error loading range pic";
	}
	if (!bomb.loadFromFile("fbomb.png"))
	{
		cout << "error loading bomb pic";
	}

	if (!hurt.loadFromFile("hurt.png"))
	{
		cout << "error loading  hurt pic";
	}
	if (!one.loadFromFile("one.png"))
	{

		cout << "error loading range pic";
	}
	if (!two.loadFromFile("two.png"))
	{

		cout << "error loading range pic";
	}
	if (!three.loadFromFile("three.png"))
	{

		cout << "error loading range pic";
	}
	

	life.setSmooth(1);
	bomb.setSmooth(1);
	hurt.setSmooth(1);
	range.setSmooth(1);


	//###############################################################################
	// Player abilities
	Sprite lifeS, bombS, rangeS, Hurt, Wallpaper;
	lifeS.setTexture(life);
	lifeS.setPosition(0, 180);
	lifeS.setScale(0.4, 0.4);

	rangeS.setTexture(range);
	rangeS.setPosition(0, 250);
	rangeS.setScale(0.4, 0.4);

	bombS.setTexture(bomb);
	bombS.setPosition(0, 330);
	bombS.setScale(0.4, 0.4);

	Hurt.setTexture(hurt);
	Wallpaper.setTexture(wallpaper);
	// Numbers
	Sprite num_of_bombs, num_of_lives, num_of_ranges;
	num_of_lives.setTexture(three);
	num_of_lives.setPosition(65, 210);
	num_of_lives.setScale(0.2, 0.2);

	num_of_bombs.setTexture(three);
	num_of_bombs.setPosition(65, 285);
	num_of_bombs.setScale(0.2, 0.2);

	num_of_ranges.setTexture(three);
	num_of_ranges.setScale(0.2, 0.2);
	num_of_ranges.setPosition(65, 360);

	//###############################################################################


	// Distribution of wall & ground on the Grid

	Sprite Grid[width + 1][length + 1];

	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= length; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{

				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));

			}

			else if (i == 1 || i == width)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else if (j == 1 || j == length)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else
			{
				if (i % 2 == j % 2) { Grid[i][j].setTexture(ground); }
				else { Grid[i][j].setTexture(ground2); }
				Grid[i][j].setPosition((i + 2) * 53, (j + 0.8) * 48);
				Grid[i][j].setScale(0.1, 0.101);
			}
		}
	}



	for (int i = 1; i <= 30; i++)
	{
		int x = rand() % 15 + 2, y = rand() % 9 + 2;
		if (x % 2 != 0 && y % 2 != 0)
		{
			x++;
		}
		Grid[x][y].setTexture(breakable);
		Grid[x][y].setPosition((x + 1.97)*53.15, (y + 0.255) * 48);
		Grid[x][y].setScale(Vector2f(0.1009, 0.165));
	}

	//###############################################################################

	// Game Window
	RenderWindow Level_1(VideoMode(1024, 680), "Fuck");
	while (Level_1.isOpen())
	{
		// Robot 1 movement
		Robot_xindx = (float)((float)(Robot.getPosition().x - 183) / 53 + 2);
		Robot_yindx = (float)((float)(Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Robot.getPosition().x + 20); right.j = Robot_yindx;
		left.i = GetXindx(Robot.getPosition().x - 20); left.j = Robot_yindx;
		up.i = Robot_xindx; up.j = GetYindx(Robot.getPosition().y - 7);
		down.i = Robot_xindx; down.j = GetYindx(Robot.getPosition().y + 7);
		//######################################################################################################
		// Robot Movement
		if (Robot_counter >= 250)
		{
			Robot_Rand_motion = rand() % 4 + 1;
			Robot_counter = 0;
		}

		if (Robot_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			Robot_counter++;
			if (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				Robot.move(0.0, -Random_Monsters_Speed);
				Robot_time = Robot_clock.getElapsedTime();
				if (Robot_time.asSeconds() >= 0.1) {
					Robot_clock.restart();
					Robot_position++;
					if (Robot_position > 8)
						Robot_position -= 8;
				}
				Robot.setTextureRect(IntRect(Yellow_Robot_size.x*Robot_position, Yellow_Robot_size.y * 0, Yellow_Robot_size.x, Yellow_Robot_size.y));

			}
			else
			{
				Robot_counter = 250;
			}

		}


		else if (Robot_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			Robot_counter++;
			if (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Robot.move(0.0, Random_Monsters_Speed);
				Robot_time = Robot_clock.getElapsedTime();
				if (Robot_time.asSeconds() >= 0.1) {
					Robot_clock.restart();
					Robot_position++;
					if (Robot_position > 8)
						Robot_position -= 8;
				}
				Robot.setTextureRect(IntRect(Yellow_Robot_size.x*Robot_position, Yellow_Robot_size.y * 2, Yellow_Robot_size.x, Yellow_Robot_size.y));

			}

			else
			{
				Robot_counter = 250;
			}
		}


		else if (Robot_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			Robot_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Robot.move(Random_Monsters_Speed, 0.0);
				Robot_time = Robot_clock.getElapsedTime();
				if (Robot_time.asSeconds() >= 0.1) {
					Robot_clock.restart();
					Robot_position++;
					if (Robot_position > 8)
						Robot_position -= 8;
				}
				Robot.setTextureRect(IntRect(Yellow_Robot_size.x*Robot_position, Yellow_Robot_size.y * 3, Yellow_Robot_size.x, Yellow_Robot_size.y));

			}
			else
			{
				Robot_counter = 250;
			}
		}


		else if (Robot_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			Robot_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Robot.move(-Random_Monsters_Speed, 0.0);
				Robot_time = Robot_clock.getElapsedTime();
				if (Robot_time.asSeconds() >= 0.1) {
					Robot_clock.restart();
					Robot_position++;
					if (Robot_position > 8)
						Robot_position -= 8;
				}
				Robot.setTextureRect(IntRect(Yellow_Robot_size.x*Robot_position, Yellow_Robot_size.y * 1, Yellow_Robot_size.x, Yellow_Robot_size.y));

			}
			else
			{
				Robot_counter = 250;
			}

		}

		// Skeleton movement
		Skeleton_xindx = (float)((float)(Skeleton.getPosition().x - 183) / 53 + 2);
		Skeleton_yindx = (float)((float)(Skeleton.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Skeleton.getPosition().x + 20); right.j = Skeleton_yindx;
		left.i = GetXindx(Skeleton.getPosition().x - 20); left.j = Skeleton_yindx;
		up.i = Skeleton_xindx; up.j = GetYindx(Skeleton.getPosition().y - 7);
		down.i = Skeleton_xindx; down.j = GetYindx(Skeleton.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// Skeleton Movement
		if (Skeleton_counter >= 250)
		{
			Skeleton_Rand_motion = rand() % 4 + 1;
			Skeleton_counter = 0;
		}

		if (Skeleton_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			Skeleton_counter++;
			if (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				Skeleton.move(0.0, -Random_Monsters_Speed);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 0, Skeleton_size.x, Skeleton_size.y));

			}
			else
			{
				Skeleton_counter = 250;
			}

		}


		else if (Skeleton_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			Skeleton_counter++;
			if (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Skeleton.move(0.0, Random_Monsters_Speed);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 2, Skeleton_size.x, Skeleton_size.y));

			}

			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			Skeleton_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Skeleton.move(Random_Monsters_Speed, 0.0);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 3, Skeleton_size.x, Skeleton_size.y));

			}
			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			Skeleton_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Skeleton.move(-Random_Monsters_Speed, 0.0);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 1, Skeleton_size.x, Skeleton_size.y));

			}
			else
			{
				Skeleton_counter = 250;
			}

		}

		//######################################################################################################
		// White_Robot movement
		White_Robot_xindx = (float)((float)(White_Robot.getPosition().x - 183) / 53 + 2);
		White_Robot_yindx = (float)((float)(White_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(White_Robot.getPosition().x + 20); right.j = White_Robot_yindx;
		left.i = GetXindx(White_Robot.getPosition().x - 20); left.j = White_Robot_yindx;
		up.i = White_Robot_xindx; up.j = GetYindx(White_Robot.getPosition().y - 7);
		down.i = White_Robot_xindx; down.j = GetYindx(White_Robot.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// White_Robot Movement
		if (White_Robot_counter >= 250)
		{
			White_Robot_Rand_motion = rand() % 4 + 1;
			White_Robot_counter = 0;
		}

		if (White_Robot_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			White_Robot_counter++;
			if (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				White_Robot.move(0.0, -Random_Monsters_Speed);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 0, White_Robot_size.x, White_Robot_size.y));

			}
			else
			{
				White_Robot_counter = 250;
			}

		}


		else if (White_Robot_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			White_Robot_counter++;
			if (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				White_Robot.move(0.0, Random_Monsters_Speed);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 2, White_Robot_size.x, White_Robot_size.y));

			}

			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			White_Robot_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				White_Robot.move(Random_Monsters_Speed, 0.0);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 3, White_Robot_size.x, White_Robot_size.y));

			}
			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			White_Robot_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				White_Robot.move(-Random_Monsters_Speed, 0.0);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 1, White_Robot_size.x, White_Robot_size.y));

			}
			else
			{
				White_Robot_counter = 250;
			}

		}

		//######################################################################################################

		// Player Movement
		Player_xindx = (float)((float)(player.getPosition().x - 183) / 53 + 2);
		Player_yindx = (float)((float)(player.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player.getPosition().x + 20); right.j = Player_yindx;
		left.i = GetXindx(player.getPosition().x - 20); left.j = Player_yindx;
		up.i = Player_xindx; up.j = GetYindx(player.getPosition().y - 7);
		down.i = Player_xindx; down.j = GetYindx(player.getPosition().y + 7);

		if (Keyboard::isKeyPressed(Keyboard::Key::Right) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			player.move(player_speed, 0.0);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 3, player_size.x, player_size.y));
		}

		else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{

			player.move(-player_speed, 0.0);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 1, player_size.x, player_size.y));

		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Up) && (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{
			player.move(0.0, -player_speed);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 0, player_size.x, player_size.y));
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Down) && (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{
			player.move(0.0, player_speed);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 2, player_size.x, player_size.y));
		}

		//######################################################################################################

		Event event;
		while (Level_1.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Enter))
				Level_1.close();

		}
		Level_1.clear();

		// Drawing of the Wallpaper
		Level_1.draw(Wallpaper);

		//Drawing of The Map
		for (int i = 2; i < width; i++)
		{
			for (int j = 2; j < length; j++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					Level_1.draw(Grid[i][j]);
				}
			}

		}

		for (int j = 1; j <= length; j++)
		{
			for (int i = 1; i <= width; i++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					continue;
				}

				Level_1.draw(Grid[i][j]);

				if (GetYindx(player.getPosition().y) >= j)
				{
					Level_1.draw(player);
				}

				if (GetYindx(Robot.getPosition().y) >= j)
				{
					Level_1.draw(Robot);
				}
				if (GetYindx(Skeleton.getPosition().y) >= j)
				{
					Level_1.draw(Skeleton);
				}
				if (GetYindx(White_Robot.getPosition().y) >= j)
				{
					Level_1.draw(White_Robot);
				}

			}
		}


		// player damage
		if (GetXindx(player.getPosition().x) == GetXindx(White_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(White_Robot.getPosition().y))
		{
			Level_1.draw(Hurt);

		}

		if (GetXindx(player.getPosition().x) == GetXindx(Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Robot.getPosition().y))
		{
			Level_1.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Skeleton.getPosition().y))
		{
			Level_1.draw(Hurt);
		}


		Level_1.draw(lifeS);
		Level_1.draw(rangeS);
		Level_1.draw(bombS);
		Level_1.draw(num_of_lives);
		Level_1.draw(num_of_ranges);
		Level_1.draw(num_of_bombs);
		Level_1.display();


	}

}
void Legendary_Mode() {
	//###############################################################################
	// Making of Player
	RectangleShape player(Vector2f(100, 120));
	Texture player_texture;
	Vector2u player_size;
	Clock player_clock;
	Time player_time;
	Generate_Charcter(player, player_texture, player_size, "mainplayer.png", 10, 7);
	//###############################################################################
	
	//###############################################################################
	// Making of Red_Monster(Smart)
	RectangleShape Red_Robot(Vector2f(100, 120));
	Texture Red_Robot_texture;
	Vector2u Red_Robot_size;
	Clock Red_Robot_clock;
	Time Red_Robot_time;
	Generate_Charcter(Red_Robot, Red_Robot_texture, Red_Robot_size, "red_robot.png", 10, 2);
	//###############################################################################
	
	//###############################################################################
	// Making of Smart_Skeleton
	RectangleShape Smart_Skeleton(Vector2f(100, 120));
	Texture Smart_Skeleton_texture;
	Vector2u Smart_Skeleton_size;
	Clock Smart_Skeleton_clock;
	Time Smart_Skeleton_time;
	Generate_Charcter(Smart_Skeleton, Smart_Skeleton_texture, Smart_Skeleton_size, "smart_skeleton.png", 10, 2);
	//###############################################################################

	//###############################################################################
	// Making of Skeleton
	RectangleShape Skeleton(Vector2f(100, 120));
	Texture Skeleton_texture;
	Vector2u Skeleton_size;
	Clock Skeleton_clock;
	Time Skeleton_time;
	Generate_Charcter(Skeleton, Skeleton_texture, Skeleton_size, "monster (5).png", 7, 8);
	//###############################################################################

	//###############################################################################
	// Making of White_Robot
	RectangleShape White_Robot(Vector2f(100, 120));
	Texture White_Robot_texture;
	Vector2u White_Robot_size;
	Clock White_Robot_clock;
	Time White_Robot_time;
	Generate_Charcter(White_Robot, White_Robot_texture, White_Robot_size, "white_robot.png", 14, 8);
	//###############################################################################

	//###############################################################################
	// Movement stuff
	leftmovement left; rightmovement right; upmovement up; downmovement down;
	int Player_xindx = 0, Player_yindx = 0, White_Robot_xindx = 0, White_Robot_yindx = 0, Skeleton_xindx = 0, Skeleton_yindx = 0, Smart_Skeleton_xindx = 0, Smart_Skeleton_yindx = 0,Red_Robot_xindx=0, Red_Robot_yindx = 0;
	//###############################################################################

 // Making of Grid
	Texture wall, ground, wallpaper, ground2, breakable, life, bomb, range, hurt, one, two, three, choice;
	if (!breakable.loadFromFile("breakable.png"))
	{
		cout << "error loading breakable wall pic";
	}
	if (!wallpaper.loadFromFile("wallpaper.jpg"))
	{
		cout << "error loading wallpaper pic";
	}
	if (!wall.loadFromFile("3d wall.png"))
	{
		cout << "error loading wall pic";
	}

	if (!ground.loadFromFile("ground 1.png"))
	{
		cout << "error loading ground pic";
	}
	if (!ground2.loadFromFile("grass.png"))
	{
		cout << "error loading grass pic";
	}

	if (!life.loadFromFile("flife.png"))
	{
		cout << "error loading life pic";
	}
	if (!range.loadFromFile("frange.png"))
	{

		cout << "error loading range pic";
	}
	if (!bomb.loadFromFile("fbomb.png"))
	{
		cout << "error loading bomb pic";
	}

	if (!hurt.loadFromFile("hurt.png"))
	{
		cout << "error loading  hurt pic";
	}
	if (!one.loadFromFile("one.png"))
	{

		cout << "error loading range pic";
	}
	if (!two.loadFromFile("two.png"))
	{

		cout << "error loading range pic";
	}
	if (!three.loadFromFile("three.png"))
	{

		cout << "error loading range pic";
	}


	life.setSmooth(1);
	bomb.setSmooth(1);
	hurt.setSmooth(1);
	range.setSmooth(1);


	//###############################################################################
	// Player abilities
	Sprite lifeS, bombS, rangeS, Hurt, Wallpaper;
	lifeS.setTexture(life);
	lifeS.setPosition(0, 180);
	lifeS.setScale(0.4, 0.4);

	rangeS.setTexture(range);
	rangeS.setPosition(0, 250);
	rangeS.setScale(0.4, 0.4);

	bombS.setTexture(bomb);
	bombS.setPosition(0, 330);
	bombS.setScale(0.4, 0.4);

	Hurt.setTexture(hurt);
	Wallpaper.setTexture(wallpaper);
	// Numbers
	Sprite num_of_bombs, num_of_lives, num_of_ranges;
	num_of_lives.setTexture(three);
	num_of_lives.setPosition(65, 210);
	num_of_lives.setScale(0.2, 0.2);

	num_of_bombs.setTexture(three);
	num_of_bombs.setPosition(65, 285);
	num_of_bombs.setScale(0.2, 0.2);

	num_of_ranges.setTexture(three);
	num_of_ranges.setScale(0.2, 0.2);
	num_of_ranges.setPosition(65, 360);

	//###############################################################################


	// Distribution of wall & ground on the Grid

	Sprite Grid[width + 1][length + 1];

	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= length; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{

				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));

			}

			else if (i == 1 || i == width)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else if (j == 1 || j == length)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else
			{
				if (i % 2 == j % 2) { Grid[i][j].setTexture(ground); }
				else { Grid[i][j].setTexture(ground2); }
				Grid[i][j].setPosition((i + 2) * 53, (j + 0.8) * 48);
				Grid[i][j].setScale(0.1, 0.101);
			}
		}
	}



	for (int i = 1; i <= 30; i++)
	{
		int x = rand() % 15 + 2, y = rand() % 9 + 2;
		if (x % 2 != 0 && y % 2 != 0)
		{
			x++;
		}
		Grid[x][y].setTexture(breakable);
		Grid[x][y].setPosition((x + 1.97)*53.15, (y + 0.255) * 48);
		Grid[x][y].setScale(Vector2f(0.1009, 0.165));
	}

	// Game Window
	RenderWindow Level_3(VideoMode(1024, 680), "Fuck");
	while (Level_3.isOpen())
	{

		// Player Movement
		Player_xindx = (float)((float)(player.getPosition().x - 183) / 53 + 2);
		Player_yindx = (float)((float)(player.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player.getPosition().x + 20); right.j = Player_yindx;
		left.i = GetXindx(player.getPosition().x - 20); left.j = Player_yindx;
		up.i = Player_xindx; up.j = GetYindx(player.getPosition().y - 7);
		down.i = Player_xindx; down.j = GetYindx(player.getPosition().y + 7);

		if (Keyboard::isKeyPressed(Keyboard::Key::Right) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			player.move(player_speed, 0.0);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 3, player_size.x, player_size.y));
		}

		else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{

			player.move(-player_speed, 0.0);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 1, player_size.x, player_size.y));

		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Up) && (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{
			player.move(0.0, -player_speed);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 0, player_size.x, player_size.y));
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Down) && (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{
			player.move(0.0, player_speed);
			player_time = player_clock.getElapsedTime();
			if (player_time.asSeconds() >= 0.1) {
				player_clock.restart();
				player_position++;
				if (player_position > 8)
					player_position -= 8;
			}
			player.setTextureRect(IntRect(player_size.x*player_position, player_size.y * 2, player_size.x, player_size.y));
		}

		//######################################################################################################



		//######################################################################################################
		// Smart_Skeleton movement
		Smart_Skeleton_xindx = (float)((float)(Smart_Skeleton.getPosition().x - 183) / 53 + 2);
		Smart_Skeleton_yindx = (float)((float)(Smart_Skeleton.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Smart_Skeleton.getPosition().x + 20); right.j = Smart_Skeleton_yindx;
		left.i = GetXindx(Smart_Skeleton.getPosition().x - 20); left.j = Smart_Skeleton_yindx;
		up.i = Smart_Skeleton_xindx; up.j = GetYindx(Smart_Skeleton.getPosition().y - 7);
		down.i = Smart_Skeleton_xindx; down.j = GetYindx(Smart_Skeleton.getPosition().y + 7);
		
		// Call BFS
		BFS(Player_xindx, Player_yindx, Smart_Skeleton_xindx, Smart_Skeleton_yindx, Grid, ground, ground2);

		// Movement
		if (Smart_Skeleton_Movement.i == Smart_Skeleton_xindx && Smart_Skeleton_Movement.j - Smart_Skeleton_yindx == 1) {
			Smart_Skeleton.move(0.0, Smart_Monsters_Speed);
			Smart_Skeleton_time = Smart_Skeleton_clock.getElapsedTime();
			if (Smart_Skeleton_time.asSeconds() >= 0.1) {
				Smart_Skeleton_clock.restart();
				Smart_Skeleton_position++;
				if (Smart_Skeleton_position > 8)
					Smart_Skeleton_position -= 8;
			}
			Smart_Skeleton.setTextureRect(IntRect(Smart_Skeleton_size.x*Smart_Skeleton_position, Smart_Skeleton_size.y * 2, Smart_Skeleton_size.x, Smart_Skeleton_size.y));
		}
		else if (Smart_Skeleton_Movement.i == Smart_Skeleton_xindx && Smart_Skeleton_Movement.j - Smart_Skeleton_yindx == -1) {
			Smart_Skeleton.move(0.0, -Smart_Monsters_Speed);
			Smart_Skeleton_time = Smart_Skeleton_clock.getElapsedTime();
			if (Smart_Skeleton_time.asSeconds() >= 0.1) {
				Smart_Skeleton_clock.restart();
				Smart_Skeleton_position++;
				if (Smart_Skeleton_position > 8)
					Smart_Skeleton_position -= 8;
			}
			Smart_Skeleton.setTextureRect(IntRect(Smart_Skeleton_size.x*Smart_Skeleton_position, Smart_Skeleton_size.y * 0, Smart_Skeleton_size.x, Smart_Skeleton_size.y));
		}
		else if (Smart_Skeleton_Movement.i - Smart_Skeleton_xindx == 1 && Smart_Skeleton_Movement.j == Smart_Skeleton_yindx) {
			Smart_Skeleton.move(Smart_Monsters_Speed, 0.0);
			Smart_Skeleton_time = Smart_Skeleton_clock.getElapsedTime();
			if (Smart_Skeleton_time.asSeconds() >= 0.1) {
				Smart_Skeleton_clock.restart();
				Smart_Skeleton_position++;
				if (Smart_Skeleton_position > 8)
					Smart_Skeleton_position -= 8;
			}
			Smart_Skeleton.setTextureRect(IntRect(Smart_Skeleton_size.x*Smart_Skeleton_position, Smart_Skeleton_size.y * 3, Smart_Skeleton_size.x, Smart_Skeleton_size.y));
		}
		else if (Smart_Skeleton_Movement.i - Smart_Skeleton_xindx == -1 && Smart_Skeleton_Movement.j == Smart_Skeleton_yindx) {

			Smart_Skeleton.move(-Smart_Monsters_Speed, 0.0);
			Smart_Skeleton_time = Smart_Skeleton_clock.getElapsedTime();
			if (Smart_Skeleton_time.asSeconds() >= 0.1) {
				Smart_Skeleton_clock.restart();
				Smart_Skeleton_position++;
				if (Smart_Skeleton_position > 8)
					Smart_Skeleton_position -= 8;
			}
			Smart_Skeleton.setTextureRect(IntRect(Smart_Skeleton_size.x*Smart_Skeleton_position, Smart_Skeleton_size.y * 1, Smart_Skeleton_size.x, Smart_Skeleton_size.y));
		}

		//######################################################################################################


		//######################################################################################################
		// Red_Robot movement
		Red_Robot_xindx = (float)((float)(Red_Robot.getPosition().x - 183) / 53 + 2);
		Red_Robot_yindx = (float)((float)(Red_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Red_Robot.getPosition().x + 20); right.j = Red_Robot_yindx;
		left.i = GetXindx(Red_Robot.getPosition().x - 20); left.j = Red_Robot_yindx;
		up.i = Red_Robot_xindx; up.j = GetYindx(Red_Robot.getPosition().y - 7);
		down.i = Red_Robot_xindx; down.j = GetYindx(Red_Robot.getPosition().y + 7);

		// Call BFS
		BFS(Player_xindx, Player_yindx, Red_Robot_xindx, Red_Robot_yindx, Grid, ground, ground2);

		// Movement
		if (Red_Robot_Movement.i == Red_Robot_xindx && Red_Robot_Movement.j - Red_Robot_yindx == 1) {
			Red_Robot.move(0.0, Red_Robot_Speed);
			Red_Robot_time = Red_Robot_clock.getElapsedTime();
			if (Red_Robot_time.asSeconds() >= 0.1) {
				Red_Robot_clock.restart();
				Red_Robot_position++;
				if (Red_Robot_position > 8)
					Red_Robot_position -= 8;
			}
			Red_Robot.setTextureRect(IntRect(Red_Robot_size.x*Red_Robot_position, Red_Robot_size.y * 2, Red_Robot_size.x, Red_Robot_size.y));
		}
		else if (Red_Robot_Movement.i == Red_Robot_xindx && Red_Robot_Movement.j - Red_Robot_yindx == -1) {
			Red_Robot.move(0.0, -Red_Robot_Speed);
			Red_Robot_time = Red_Robot_clock.getElapsedTime();
			if (Red_Robot_time.asSeconds() >= 0.1) {
				Red_Robot_clock.restart();
				Red_Robot_position++;
				if (Red_Robot_position > 8)
					Red_Robot_position -= 8;
			}
			Red_Robot.setTextureRect(IntRect(Red_Robot_size.x*Red_Robot_position, Red_Robot_size.y * 0, Red_Robot_size.x, Red_Robot_size.y));
		}
		else if (Red_Robot_Movement.i - Red_Robot_xindx == 1 && Red_Robot_Movement.j == Red_Robot_yindx) {
			Red_Robot.move(Red_Robot_Speed, 0.0);
			Red_Robot_time = Red_Robot_clock.getElapsedTime();
			if (Red_Robot_time.asSeconds() >= 0.1) {
				Red_Robot_clock.restart();
				Red_Robot_position++;
				if (Red_Robot_position > 8)
					Red_Robot_position -= 8;
			}
			Red_Robot.setTextureRect(IntRect(Red_Robot_size.x*Red_Robot_position, Red_Robot_size.y * 3, Red_Robot_size.x, Red_Robot_size.y));
		}
		else if (Red_Robot_Movement.i - Red_Robot_xindx == -1 && Red_Robot_Movement.j == Red_Robot_yindx) {

			Red_Robot.move(-Red_Robot_Speed, 0.0);
			Red_Robot_time = Red_Robot_clock.getElapsedTime();
			if (Red_Robot_time.asSeconds() >= 0.1) {
				Red_Robot_clock.restart();
				Red_Robot_position++;
				if (Red_Robot_position > 8)
					Red_Robot_position -= 8;
			}
			Red_Robot.setTextureRect(IntRect(Red_Robot_size.x*Red_Robot_position, Red_Robot_size.y * 1, Red_Robot_size.x, Red_Robot_size.y));
		}

		//######################################################################################################



		
		// Skeleton movement
		Skeleton_xindx = (float)((float)(Skeleton.getPosition().x - 183) / 53 + 2);
		Skeleton_yindx = (float)((float)(Skeleton.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Skeleton.getPosition().x + 20); right.j = Skeleton_yindx;
		left.i = GetXindx(Skeleton.getPosition().x - 20); left.j = Skeleton_yindx;
		up.i = Skeleton_xindx; up.j = GetYindx(Skeleton.getPosition().y - 7);
		down.i = Skeleton_xindx; down.j = GetYindx(Skeleton.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// Skeleton Movement
		if (Skeleton_counter >= 250)
		{
			Skeleton_Rand_motion = rand() % 4 + 1;
			Skeleton_counter = 0;
		}

		if (Skeleton_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			Skeleton_counter++;
			if (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				Skeleton.move(0.0, -Random_Monsters_Speed);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 0, Skeleton_size.x, Skeleton_size.y));

			}
			else
			{
				Skeleton_counter = 250;
			}

		}


		else if (Skeleton_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			Skeleton_counter++;
			if (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Skeleton.move(0.0, Random_Monsters_Speed);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 2, Skeleton_size.x, Skeleton_size.y));

			}

			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			Skeleton_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Skeleton.move(Random_Monsters_Speed, 0.0);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 3, Skeleton_size.x, Skeleton_size.y));

			}
			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			Skeleton_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Skeleton.move(-Random_Monsters_Speed, 0.0);
				Skeleton_time = Skeleton_clock.getElapsedTime();
				if (Skeleton_time.asSeconds() >= 0.1) {
					Skeleton_clock.restart();
					Skeleton_position++;
					if (Skeleton_position > 8)
						Skeleton_position -= 8;
				}
				Skeleton.setTextureRect(IntRect(Skeleton_size.x*Skeleton_position, Skeleton_size.y * 1, Skeleton_size.x, Skeleton_size.y));

			}
			else
			{
				Skeleton_counter = 250;
			}

		}

		//######################################################################################################
		// White_Robot movement
		White_Robot_xindx = (float)((float)(White_Robot.getPosition().x - 183) / 53 + 2);
		White_Robot_yindx = (float)((float)(White_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(White_Robot.getPosition().x + 20); right.j = White_Robot_yindx;
		left.i = GetXindx(White_Robot.getPosition().x - 20); left.j = White_Robot_yindx;
		up.i = White_Robot_xindx; up.j = GetYindx(White_Robot.getPosition().y - 7);
		down.i = White_Robot_xindx; down.j = GetYindx(White_Robot.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// White_Robot Movement
		if (White_Robot_counter >= 250)
		{
			White_Robot_Rand_motion = rand() % 4 + 1;
			White_Robot_counter = 0;
		}

		if (White_Robot_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			White_Robot_counter++;
			if (Grid[up.i][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				White_Robot.move(0.0, -Random_Monsters_Speed);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 0, White_Robot_size.x, White_Robot_size.y));

			}
			else
			{
				White_Robot_counter = 250;
			}

		}


		else if (White_Robot_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			White_Robot_counter++;
			if (Grid[down.i][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				White_Robot.move(0.0, Random_Monsters_Speed);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 2, White_Robot_size.x, White_Robot_size.y));

			}

			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			White_Robot_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				White_Robot.move(Random_Monsters_Speed, 0.0);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 3, White_Robot_size.x, White_Robot_size.y));

			}
			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			White_Robot_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				White_Robot.move(-Random_Monsters_Speed, 0.0);
				White_Robot_time = White_Robot_clock.getElapsedTime();
				if (White_Robot_time.asSeconds() >= 0.1) {
					White_Robot_clock.restart();
					White_Robot_position++;
					if (White_Robot_position > 8)
						White_Robot_position -= 8;
				}
				White_Robot.setTextureRect(IntRect(White_Robot_size.x*White_Robot_position, White_Robot_size.y * 1, White_Robot_size.x, White_Robot_size.y));

			}
			else
			{
				White_Robot_counter = 250;
			}

		}

		//######################################################################################################

		

		Event event;
		while (Level_3.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Enter))
				Level_3.close();

		}
		Level_3.clear();

		// Drawing of the Wallpaper
		Level_3.draw(Wallpaper);

		//Drawing of The Map
		for (int i = 2; i < width; i++)
		{
			for (int j = 2; j < length; j++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					Level_3.draw(Grid[i][j]);
				}
			}

		}

		for (int j = 1; j <= length; j++)
		{
			for (int i = 1; i <= width; i++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					continue;
				}

				Level_3.draw(Grid[i][j]);

				if (GetYindx(player.getPosition().y) >= j)
				{
					Level_3.draw(player);
				}

				if (GetYindx(Skeleton.getPosition().y) >= j)
				{
					Level_3.draw(Skeleton);
				}

				if (GetYindx(White_Robot.getPosition().y) >= j)
				{
					Level_3.draw(White_Robot);
				}

				if (GetYindx(Smart_Skeleton.getPosition().y) >= j)
				{
					Level_3.draw(Smart_Skeleton);
				}

				if (GetYindx(Red_Robot.getPosition().y) >= j)
				{
					Level_3.draw(Red_Robot);
				}
			}
		}


		// player damage
		if (GetXindx(player.getPosition().x) == GetXindx(White_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(White_Robot.getPosition().y))
		{
			Level_3.draw(Hurt);

		}

		if (GetXindx(player.getPosition().x) == GetXindx(Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Skeleton.getPosition().y))
		{
			Level_3.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Smart_Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Smart_Skeleton.getPosition().y))
		{
			Level_3.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Red_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Red_Robot.getPosition().y))
		{
			Level_3.draw(Hurt);
		}


		Level_3.draw(lifeS);
		Level_3.draw(rangeS);
		Level_3.draw(bombS);
		Level_3.draw(num_of_lives);
		Level_3.draw(num_of_ranges);
		Level_3.draw(num_of_bombs);
		Level_3.display();


	}

}

int main()
{
	
	//###############################################################################
	// Menu Shits
	RenderWindow background(VideoMode(1024, 680), "Fuck");
	setmenustatus(background.getSize().x, background.getSize().y);
	Texture backgroundwall;
	backgroundwall.loadFromFile("FantasyLand.png");
	Sprite Menuwall; Menuwall.setTexture(backgroundwall);
	//Menu choices
	Sprite Choice, Choice2;
	Texture choice;
	if (!choice.loadFromFile("Untitled-1-Recovered.png"))
	{
		cout << "choice not found";
	}
	Choice.setTexture(choice);
	Choice.setPosition(0, 415);
	Choice.setScale(0.3, 0.27);

	Choice2.setTexture(choice);
	Choice2.setPosition(1024 - (0.3 * 1307), 315);
	Choice2.setScale(0.3, 0.27);
	//###############################################################################

	

	while (background.isOpen())
	{
		// Menu choices Animation
		if (Choice.getPosition().x < 310)
		{
			Choice.move(0.5, 0);
		}
		if (Choice2.getPosition().x > 310)
		{
			Choice2.move(-0.5, 0);
		}

		Event evnt;
		while (background.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::KeyReleased:
				switch (evnt.key.code)
				{
				case sf::Keyboard::Up:
					MoveUp();
					break;

				case sf::Keyboard::Down:
					MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (GetPressedItem())
					{
					case 0:
						background.close();
						break;
					case 1:
						background.close();
						break;
					case 2:
						cout << "About Us button has been pressed \n";
					case 3:
						background.close();
						//window.close();
						return(0);
						break;
					}

					break;
				}

				break;
			case Event::Closed:
				background.close();
				return 0;
				break;
			}
		}

		if ((Mouse::isButtonPressed(Mouse::Left)) && (Mouse::getPosition().x >= 615 && Mouse::getPosition().x <= 680)) {
			background.close();
			break;

		}
		background.clear();
		background.draw(Menuwall);
		background.draw(Choice);
		background.draw(Choice2);
		drawmenu(background);
		background.display();

	}
	RenderWindow levels(VideoMode(1024, 680), "levels");
	setlevelsstatus(levels.getSize().x, levels.getSize().y);
	Texture levelwall;
	levelwall.loadFromFile("levelwall.png");
	Sprite levelwallS; levelwallS.setTexture(levelwall);
	Choice2.setPosition(1024 - (0.3 * 1307), 315); Choice.setPosition(0, 415);
	while (levels.isOpen())
	{
		// Menu choices Animation
		if (Choice.getPosition().x < 310)
		{
			Choice.move(0.5, 0);
		}
		if (Choice2.getPosition().x > 310)
		{
			Choice2.move(-0.5, 0);
		}

		Event evntlev;
		while (levels.pollEvent(evntlev))
		{
			switch (evntlev.type)
			{
			case Event::KeyReleased:
				switch (evntlev.key.code)
				{
				case Keyboard::Up:
					MoveUplev();
					break;

				case Keyboard::Down:
					MoveDownlev();
					break;

				case Keyboard::Return:
					switch (GetPressedlevel())
					{
					case 0:
						levels.close();
						Beginner_Mode();
						break;
					case 1:
						return 0;
						break;
					case 2:
						levels.close();
						Legendary_Mode();
						break;
					}

					break;
				}

				break;
			case Event::Closed:
				background.close();
				return 0;
				break;
			}
			

		}
		if ((Mouse::isButtonPressed(Mouse::Left)) && (Mouse::getPosition().x >= 615 && Mouse::getPosition().x <= 680)) {
			background.close();
			break;

		}

		levels.clear();
		levels.draw(levelwallS);
		levels.draw(Choice);
		levels.draw(Choice2);
		drawlev(levels);
		levels.display();
		
	}
	
	


}
