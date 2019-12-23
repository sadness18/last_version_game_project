#include <iostream> //���-��
#include <sstream> //��� ���-��
#include <list> //���������� ������ ���������� STL
#include <SFML/Graphics.hpp> //���������� ������� sfml
#include <ctime> //���������� �����
#include <math.h> //���������� � ��������������� ���������
#include "map.h" //���������� ���� � ������
#include "life_bar.h" //���������� ���� � �������, ���������� �� ���������� ����� ��������
#include "menu.h"
#include "class.h"

using namespace std; //������������ ���� std, ����� �� ������ ������ ��� std::
using namespace sf;

/////////ENTITY//////////

entity::entity(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) {
	x = X; y = Y; w = W; h = H; name = Name; movetimer = 0; movetimer_buff = 0; movetimer_stun = 0; randomx = 0; movetimer_health = 0;
	speed = 0; health = 100; dx = Dx; dy = Dy; sweem = 0; bubbles = 0; check_bubble_fly = 0; check_shark_stun = 0; count_gametime = 0;
	count_get_eat = 0; count_get_buff = 0; count_stun_shark = 0;
	life = true; eaten = false; kusy = false; buff = false; bubble_crash = false; stun = false; buff_drop = false; pause = false;
	pre_start = true;
	texture.loadFromImage(image);
	sprite.setTexture(texture);
}

FloatRect entity::getrect()
{
	return FloatRect(x, y, w, h);
}

void entity::set_sprite(int q, int w, int e, int r)
{
	sprite.setTextureRect(IntRect(q, w, e, r));
}

bool entity::get_eaten()
{
	return eaten;
}

void entity::set_eaten(bool arg)
{
	eaten = arg;
}

bool entity::get_bubble_crash()
{
	return bubble_crash;
}

void entity::set_bubble_crash(bool BC)
{
	bubble_crash = BC;
}

bool entity::get_stun()
{
	return stun;
}

void entity::set_stun(bool Stun)
{
	stun = Stun;
}

float entity::get_movetimer_stun()
{
	return movetimer_stun;
}

void entity::set_movetimer_stun(float Stun)
{
	movetimer_stun = Stun;
}

void entity::inc_movetimer_stun(float time)
{
	movetimer_stun += time;
}

int entity::get_check_shark_stun()
{
	return check_shark_stun;
}

/////////PLAYER//////////

player::player(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name) //�����������
{
	sprite.setTextureRect(IntRect(0, 0, w, h));
}

void player::control(float time, float Dx, float Dy) //����������� ���������
{
	x += Dx * time;
	y += Dy * time;
	speed = 0;
	sprite.setPosition(x, y);
	physical_obj(Dx, Dy);
}

void player::physical_obj(float Dx, float Dy) //�������� �� ������������ � ��������
{
	for (int i = y / 64; i < (y + h) / 64; i++)
	{
		for (int j = x / 64; j < (x + w) / 64; j++)
		{
			if (TileMap[i][j] == '0' || TileMap[i][j] == 'd')
			{
				if (Dy > 0) y = i * 64 - h;
				if (Dy < 0) y = i * 64 + 64;
				if (Dx > 0) x = j * 64 - w;
				if (Dx < 0) x = j * 64 + 64;
			}
		}
	}
}

int player::generatorhavki()
{
	randomx = 84 + rand() % 1836; //��������� ���. ����� �����, ������ ��� ����� ��������� ����������� �����
	std::cout << randomx << std::endl;	 // � -20 �������� ����� � ������ �� ����� ������� ������� (����� ������ FHD ����� 1920 �� ����
	return randomx;				//  � ��� ������������ � ��������� � 84-�� �� 1836 ������� ������������)
}

void player::update(float time)
{
	if (life) //���� ����� ��� (life = true), �� �� ����� ���������
	{
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) //�������� ����� (������ A)
		{
			sprite.setTextureRect(IntRect(60, 0, -60, 42)); //��������� ������ ���� �����
			check_bubble_fly = 1; //��� ����� ��� ����, ����� ��� �������� �������� ���������, ���� ��������� ����
			speed = 0.15; //�������� ��������
			dx = -speed; dy = 0; //�������� �� �����������
			control(time, dx, dy); //�������� �-�� control � �������� �� ����� � �������� �� �����������
		}
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) //�������� ������ (������ D)
		{
			sprite.setTextureRect(IntRect(0, 0, 60, 42)); //��������� ������ ���� ������
			check_bubble_fly = 2; //��� ����� ��� ����, ����� ��� �������� �������� ���������, ���� ��������� ����
			speed = 0.15; //�������� ��������
			dx = speed; dy = 0; //�������� �� �����������
			control(time, dx, dy); //�������� �-�� control � �������� �� ����� � �������� �� �����������
		}
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) //�������� ����� (������ W)
		{
			speed = 0.15; //�������� ��������
			dx = 0; dy = -speed; //�������� �� �����������
			control(time, dx, dy); //�������� �-�� control � �������� �� ����� � �������� �� �����������
		}
		if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) //�������� ���� (������ S)
		{
			speed = 0.15; //�������� ��������
			dx = 0; dy = speed; //�������� �� �����������
			control(time, dx, dy); //�������� �-�� control � �������� �� ����� � �������� �� �����������
		}
	}
	if (health <= 0) { life = false; } //���� �������� <= 0, �� ������
	if (health > 100) health = 100; //���� �������� ������������ �� 100 ������ �������� = 100, ��� ������ 100 ���� �� ������
	if (!life) health = 0; //��� ����, ����� �� � ����� �� ������� (���� �����, �� = 0)

	sweem += time; //���������� � ������� ��� ������� ��������
	if (sweem > 150) sprite.setPosition(x, y + 3);
	if (sweem > 300) sprite.setPosition(x, y + 3);
	if (sweem > 450) sprite.setPosition(x, y - 3);
	if (sweem > 600) { sprite.setPosition(x, y - 3); sweem = 0; }

	movetimer += time; //������� ������� (�������� ����������� � ���� 2 �������, ���� �������� ������������� � ������)
	if (kusy == true) //���� ���-���� ����
	{
		for (int i = 1; i <= 10; i++) //������� �� 1 �� 10 ������������ (��� ������� �������� ������������)
		{
			if (i % 2 != 0) //���� �������� ��������
			{
				if (movetimer > 200 * (i - 1) && movetimer < 200 * i) { sprite.setColor(Color::Transparent); } //������ ����������
			}
			else //�����, ���� ������
			{
				if (movetimer > 200 * (i - 1) && movetimer < 200 * i) { sprite.setColor(Color::White); } //������ �������
			}
		}
		if (movetimer > 2000) //���� ��� ������ 2 �������
		{
			sprite.setColor(Color::White); //������ ������ �������
			kusy = false; //���� ��������, ���� ������
			movetimer = 0; //�������� �������
		}
	}
	else movetimer = 0; //���� �� ����, �� � ����� ������� �����
}

Sprite player::get_sprite()
{
	return sprite;
}

String player::get_name()
{
	return name;
}

bool player::get_life()
{
	return life;
}

void player::dec_health(int znach)
{
	health -= znach;
}

void player::inc_health(int znach)
{
	health = health += znach;
}

int player::get_health()
{
	return health;
}

void player::set_kusy(bool Kusy)
{
	kusy = Kusy;
}

bool player::get_kusy()
{
	return kusy;
}

float player::get_movetimer_buff()
{
	return movetimer_buff;
}

void player::set_movetimer_buff(float Movetimerbuff)
{
	movetimer_buff = Movetimerbuff;
}

void player::inc_movetimer_buff(float time)
{
	movetimer_buff += time;
}

bool player::get_buff()
{
	return buff;
}

void player::set_buff(bool Buff)
{
	buff = Buff;
}

void player::set_bubbles()
{
	bubbles = 2;
}

void player::dec_bubbles()
{
	bubbles -= 1;
}

int player::get_bubbles()
{
	return bubbles;
}

int player::get_check_bubble_fly()
{
	return check_bubble_fly;
}

bool player::get_buff_drop()
{
	return buff_drop;
}

void player::set_buff_drop(bool BD)
{
	buff_drop = BD;
}

void player::set_pause()
{
	pause = !pause;
}

bool player::get_pause()
{
	return pause;
}

int player::get_count_gametime()
{
	return count_gametime;
}

void player::inc_count_gametime()
{
	count_gametime++;
}

bool player::get_pre_start()
{
	return pre_start;
}

void player::set_pre_start()
{
	pre_start = !pre_start;
}

void player::inc_movetimer_health(float znach)
{
	movetimer_health += znach;
}

void player::zero_movetimer_health()
{
	movetimer_health = 0;
}

float player::get_movetimer_health()
{
	return movetimer_health;
}

int player::get_count_get_eat()
{
	return count_get_eat;
}

int player::get_count_get_buff()
{
	return count_get_buff;
}

int player::get_count_stun_shark()
{
	return count_stun_shark;
}

void player::inc_count_get_eat()
{
	count_get_eat++;
}

void player::inc_count_get_buff()
{
	count_get_buff++;
}

void player::inc_count_stun_shark()
{
	count_stun_shark++;
}

void player::draw_map(RenderWindow& window, Sprite s_map)
{
	for (int i = 0; i < height_map; i++) //����� ��� ���������� �����
	{
		for (int j = 0; j < width_map; j++)
		{
			if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 64, 64)); //������� ����
			if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 128, 64)); //���������(�����������)
			if (TileMap[i][j] == 'd') s_map.setTextureRect(IntRect(128, 0, 192, 64)); //����� ����� (�����������)
			if (TileMap[i][j] == 'k') s_map.setTextureRect(IntRect(192, 0, 256, 64)); //�������� (�����)
			if (TileMap[i][j] == 'c') s_map.setTextureRect(IntRect(0, 64, 64, 128)); //���� (�����)
			if (TileMap[i][j] == 'y') s_map.setTextureRect(IntRect(64, 64, 128, 128)); //���� (�����)
			if (TileMap[i][j] == 'u') s_map.setTextureRect(IntRect(128, 64, 192, 128)); //������ �������������� (�����)
			if (TileMap[i][j] == 'U') s_map.setTextureRect(IntRect(192, 64, 256, 128)); //������ ������������ (�����)
			if (TileMap[i][j] == '1') s_map.setTextureRect(IntRect(0, 128, 64, 192)); //���������� ������� (�����)
			if (TileMap[i][j] == '2') s_map.setTextureRect(IntRect(64, 128, 128, 192)); //���������� ������ (�����)
			if (TileMap[i][j] == '3') s_map.setTextureRect(IntRect(128, 128, 192, 192)); //���������� ������ (�����)
			if (TileMap[i][j] == '4') s_map.setTextureRect(IntRect(192, 128, 256, 192)); //���������� ������ (�����)

			s_map.setPosition(j * 64, i * 64); //����������� ����� �������
			window.draw(s_map); //������ �����
		}
	}
}

//////////ENEMY//////////

enemy::enemy(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name) //�����������
{
	sprite.setTextureRect(IntRect(0, 0, w, h));
	//dx = 0.2; //��������� ��������
}

void enemy::physical_obj(float Dx, float Dy) //�������� �� ������������ � ��������
{
	for (int i = y / 64; i < (y + h) / 64; i++)
	{
		for (int j = x / 64; j < (x + w) / 64; j++)
		{
			if (TileMap[i][j] == '0')
			{
				if (Dy > 0) y = i * 64 - h;
				if (Dy < 0) y = i * 64 + 64;
				if (Dx > 0)
				{
					x = j * 64 - w; dx = -Dx; //���� ����� ��������� � �����������, �� ������������� �� (�����)
					sprite.setTextureRect(IntRect(126, 0, -126, 48)); //������������� ������ �����
					check_shark_stun = 1; //��� �������� ����, � ����� ������� ����� ������ ���������� �����
				}
				if (Dx < 0)
				{
					x = j * 64 + 64; dx = -Dx; //���� ����� ��������� � �����������, �� ������������� �� (������)
					sprite.setTextureRect(IntRect(0, 0, 126, 48));//������������� ������ ������
					check_shark_stun = 2; //��� �������� ����, � ����� ������� ����� ������ ���������� �����
				}
			}
		}
	}
}

void enemy::update(float time)
{
	x += dx * time;
	physical_obj(dx, 0);
	sprite.setPosition(x, y);

	if (name == "enemy_1" || name == "enemy_2") //��� ���� �1 � �2
	{
		sweem += time; //���������� � ������� ��� ������� ��������
		if (sweem > 100) sprite.setPosition(x, y + 3); //+150
		if (sweem > 250) sprite.setPosition(x, y + 3); //+300
		if (sweem > 550) sprite.setPosition(x, y - 3); //+450
		if (sweem > 1000) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
	if (name == "enemy_3" || name == "enemy_7") //��� ���� �3 � �7
	{
		sweem += time; //���������� � ������� ��� ������� ��������
		if (sweem > 100) sprite.setPosition(x, y + 3); //+100
		if (sweem > 200) sprite.setPosition(x, y + 3); //+100
		if (sweem > 300) sprite.setPosition(x, y - 3); //+100
		if (sweem > 400) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
	if (name == "enemy_5" || name == "enemy_9") //��� ���� �5 � �9
	{
		sweem += time; //���������� � ������� ��� ������� ��������
		if (sweem > 220) sprite.setPosition(x, y + 3); //+100
		if (sweem > 320) sprite.setPosition(x, y + 3); //+240
		if (sweem > 560) sprite.setPosition(x, y - 3); //+340
		if (sweem > 900) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
	if (name == "enemy_4" || name == "enemy_6" || name == "enemy_8" || name == "enemy_10") //��� ���� �4, �6, �8 � �10
	{
		sweem += time; //���������� � ������� ��� ������� ��������
		if (sweem > 150) sprite.setPosition(x, y + 3); //+150
		if (sweem > 300) sprite.setPosition(x, y + 3); //+150
		if (sweem > 450) sprite.setPosition(x, y - 3); //+150
		if (sweem > 600) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
}

Sprite enemy::get_sprite()
{
	return sprite;
}

String enemy::get_name()
{
	return name;
}

//////////OBJECT//////////

object::object(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name)
{
	sprite.setTextureRect(IntRect(0, 0, w, h));
	//dy = 0.25; //��������� ��������
}

void object::physical_obj(float Dx, float Dy) //�������� �� ������������ � ��������
{
	for (int i = y / 64; i < (y + h) / 64; i++)
	{
		for (int j = x / 64; j < (x + w) / 64; j++)
		{
			if (TileMap[i][j] == '0' || TileMap[i][j] == 'd')
			{
				if (Dy > 0) y = i * 64 - h;
				if (Dy < 0) y = i * 64 + 64;
				if (Dx > 0) x = j * 64 - w;
				if (Dx < 0) x = j * 64 + 64;
			}
		}
	}
}

void object::update(float time)
{
	y += dy * time;
	physical_obj(0, dy);
	sprite.setPosition(x, y);
}

Sprite object::get_sprite()
{
	return sprite;
}

String object::get_name()
{
	return name;
}

//////////CBUBBLE//////////

cbubble::cbubble(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name)
{
	sprite.setTextureRect(IntRect(0, 0, w, h));
	//dx = 0.35; //�������� ������ �������
}

void cbubble::physical_obj(float Dx, float Dy) //�������� �� ������������ � ��������
{
	for (int i = y / 64; i < (y + h) / 64; i++)
	{
		for (int j = x / 64; j < (x + w) / 64; j++)
		{
			if (TileMap[i][j] == '0' || TileMap[i][j] == 'd')
			{
				/*if (Dy > 0) y = i * 64 - h;
				if (Dy < 0) y = i * 64 + 64;*/
				if (Dx > 0) //���� ��������� �����
				{
					x = j * 64 - w; //���������������
					bubble_crash = true; //��������, ��� ���������
				}
				if (Dx < 0) //���� ��������� ������
				{
					x = j * 64 + 64; //���������������
					bubble_crash = true; //��������, ��� ���������
				}
			}
		}
	}
}

void cbubble::update(float time)
{
	x += dx * time;
	physical_obj(dx, 0);
	sprite.setPosition(x, y);
}

Sprite cbubble::get_sprite()
{
	return sprite;
}

String cbubble::get_name()
{
	return name;
}

//////////FUNCIONS//////////

bool is_game(RenderWindow& window) //�-�� is_game, ����������� � �������� ��������� ������ �� ������ ���� �� �-�� main. ��� �� ��� � ����������
{
	setlocale(LC_ALL, "rus");
	View view; //������ ������.
	view.reset(FloatRect(64, 64, 1920, 1080)); //������������� ������. ��������� ������, ������� � ����� 64, 64, ������ ������ 1920�1080

	Font font; //������ �����
	font.loadFromFile("18386.ttf"); //�������� ���� ������
	Text text("", font, 20); //������ ����� (������, �����, ������)
	text.setOutlineColor(Color::Black); //������ ���� ������ (������� �����)
	text.setFillColor(Color::Black); //������ ���� ������ (�������)

	Image map_image; //������ ����������� (��� �����)
	map_image.loadFromFile("images/map.png"); //��������� �������� ��������� �����
	Texture t_map; //������ ��������
	t_map.loadFromImage(map_image); //��������� ����������� � ��������
	Sprite s_map; //������ ������
	s_map.setTexture(t_map); //��������� �������� � ������

	Image heroimage; //������ ����������� (��� ������)
	heroimage.loadFromFile("images/fish.png"); //�������� ����������� ��� ������

	Image enemyoneimage; //������ ����������� (��� �����)
	enemyoneimage.loadFromFile("images/shark.png"); //�������� ����������� ��� �����

	Image eatimage; //������ ����������� (��� ���)
	eatimage.loadFromFile("images/eat.png"); //�������� ����������� ��� ���

	Image buffimage; //������ ����������� (��� ����)
	buffimage.loadFromFile("images/bubble_bonus.png"); //�������� ����������� ��� ����

	Image ammoimage; //������ ����������� (��� �������� �������)
	ammoimage.loadFromFile("images/bubble_shot_2.png"); //�������� ����������� ��� ������� ������
	Texture ammo_count_texture; //������ �������� ��� ���-�� �������� � ��� (��� �����)
	ammo_count_texture.loadFromImage(ammoimage); //�������� ����������� (����� ��, ��� � ��� ����� ��������)
	Sprite ammo_count_sprite; //������� ������, ����� ���������� ���-�� ��������� � ��� ��������
	ammo_count_sprite.setTexture(ammo_count_texture); //�������� � ������ �������� ��������

	Texture ps3_texture; //������ ������ �������� ��� ����� 3 ����� ������� ����
	ps3_texture.loadFromFile("images/ps_3.png"); //������ ��� ���� ����������� ��� ����� 3
	Sprite ps3_sprite; //������ ������ ������ ��� ����� 3
	ps3_sprite.setTexture(ps3_texture); //������ �������� �������

	Texture ps2_texture; //������ ������ �������� ��� ����� 2 ����� ������� ����
	ps2_texture.loadFromFile("images/ps_2.png"); //������ ��� ���� ����������� ��� ����� 2
	Sprite ps2_sprite; //������ ������ ������ ��� ����� 2
	ps2_sprite.setTexture(ps2_texture); //������ �������� �������

	Texture ps1_texture; //������ ������ �������� ��� ����� 1 ����� ������� ����
	ps1_texture.loadFromFile("images/ps_1.png"); //������ ��� ���� ����������� ��� ����� 1
	Sprite ps1_sprite; //������ ������ ������ ��� ����� 1
	ps1_sprite.setTexture(ps1_texture); //������ �������� �������

	Texture ps0_texture; //������ ������ �������� ��� ������� ����� ������� ����
	ps0_texture.loadFromFile("images/ps_0.png"); //������ ��� ���� ����������� ��� �������
	Sprite ps0_sprite; //������ ������ ������ ��� �������
	ps0_sprite.setTexture(ps0_texture); //������ �������� �������

	list<entity*> entities; //������������ ������ ��� ��������� ���� ������
	list<entity*>::iterator it; //�������� ��� ����� ������
	list<entity*>::iterator it_2; //������ ��������. ����� ��� ����, ����� ��������� 2 ������ ������ ������������ (��� �������� ���������)

	entities.push_back(new enemy(enemyoneimage, 1856, 64, 126, 48, 0.3, 0, "enemy_1")); //���� 1
	entities.push_back(new enemy(enemyoneimage, 64, 128, 126, 48, 0.3, 0, "enemy_2")); //���� 2
	entities.push_back(new enemy(enemyoneimage, 64, 320, 126, 48, 0.22, 0, "enemy_3")); //���� 3
	entities.push_back(new enemy(enemyoneimage, 320, 576, 126, 48, 0.2, 0, "enemy_4")); //���� 4
	entities.push_back(new enemy(enemyoneimage, 64, 832, 126, 48, 0.27, 0, "enemy_5")); //���� 5
	entities.push_back(new enemy(enemyoneimage, 768, 704, 126, 48, 0.24, 0, "enemy_6")); //���� 6
	entities.push_back(new enemy(enemyoneimage, 1344, 256, 126, 48, 0.22, 0, "enemy_7")); //���� 7
	entities.push_back(new enemy(enemyoneimage, 1152, 384, 126, 48, 0.22, 0, "enemy_8")); //���� 8
	entities.push_back(new enemy(enemyoneimage, 1856, 512, 126, 48, 0.22, 0, "enemy_9")); //���� 9
	entities.push_back(new enemy(enemyoneimage, 1600, 768, 126, 48, 0.2, 0, "enemy_10")); //���� 10

	player p(heroimage, 64, 1024, 60, 42, 0.15, 0, "player_1"); //������� ��������� (������ ��������� player)
	life_bar hp; //������ ������ life_bar ��� ����������� ������ ��������

	srand(time(0)); //������������ (����� ����������� ���������������� �����)

	list<entity*> objlist; //������, �������� � ���� ������ �� ������������ ������� ������ object (���, ���)
	for (int i = 0; i <= 2; i++) //�������������� ��������� ������� (3 ��.)
	{
		objlist.push_back(new object(eatimage, p.generatorhavki(), 64, 20, 22, 0, 0.25, "eat_1")); //������� ������� � ������ object 3 �������� (3 ���)
	}

	list<entity*> ammolist; //������, �������� ������ �������

	menu m; //������ ������ menu

	Clock clock; //���������� �������
	int minutes = 0; //���-�� �����, ��������� � ������� ������ ����
	int seconds = 0; //���-�� ������, ������� ����������� � ������, ����� ��������� 60
	int save_time = 0; //��� ���������� �������
	int count_pre_start = 0; //��� ���������� ������� ����� ������� ����
	int num_pre_start = 0; //����� ��������, ������� ���������� �������� �� �����. �������� �������� ������� �����, � ����������� � �����

	  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 ////////////////////////////////////////////////////���� ������� ����////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen()) //���� "���� ������� ����". ���, ��� � ��� ��������, ���������� � ���������� ����������� (������ ����� (���))
	{
		float time; //���������� ��� ��������� � ��� �������, ��� ����������� ���������� ������������ �������� (�.�. �������� �� �������)



		  /*------------------------------------------------------------------------------------------------*/
		 /*--------------------���������� ������� ��� �������� � ��������� (���� �����)--------------------*/
		/*------------------------------------------------------------------------------------------------*/
		if (!p.get_pause()) //���� ����� �� �����
			time = clock.getElapsedTime().asMicroseconds(); //���� ��������� ����� � �������������
		else //���� ����� �����
			time = 0; //�������� �����, ����� ������ �� ��������� �� ������
		/*------------------------------------------------------------------------------------------------*/



		clock.restart(); //������������� ����� (��������)
		time = time / 800; //�������� ����. ��� ������� (�����������) --> 1 000 000 ��� (1 �) / 800 = 1250 ��� (1 �)



		  /*------------------------------------------------------------------------*/
		 /*--------------------���������� �������� ������� ����--------------------*/
		/*------------------------------------------------------------------------*/
		if (p.get_life() && !p.get_pause() && !p.get_pre_start()) //���� ����������� 3 ������� �����: ����� ���, ����� �� ����� � ���� ��������
		{
			save_time += time; //� ���������� save_time ����������� ������������
			if (save_time >= 1250) //���� ���������� 1250 ��� ��� ������ (1 �������)
			{
				p.inc_count_gametime(); //�������������� ���������� count_gametime. ��� � ����������� � ���� ������� �����
				save_time = 0; //�������� save_time, ����� ����� ����������� � ��� 1250 ���. � ��� ���������, ���� ����������� �������
			}
		}
		minutes = floor(p.get_count_gametime() / 60); //��� ������ �� ����� ������� � ������� (��� ����� � �����, ��� ��������� ��������)
		seconds = p.get_count_gametime() % 60; //��� ������ �� ����� ������� � �������� (�� 60) (��� ����� � �����, ��� ��������� ��������)
		/*------------------------------------------------------------------------*/



		  /*-----------------------------------------------------------------------------------------*/
		 /*--------------------���������� 3-� ��������� ���������� ����� �������--------------------*/
		/*-----------------------------------------------------------------------------------------*/
		count_pre_start += time; //����������� ����� � ���������� count_pre_start, ����� � ����������� ����� ������ �� ������ ����
		if (p.get_pre_start()) //��������� ���������� pre_start �� �������� true
		{
			time = 0; //�������� �����, ����� ����� � ����� �� ���������, ���� ������ �� ���������� � ���� �� ��������
			if (count_pre_start <= 1250) //���� ���������� count_pre_start ��� �� �������� � ���� 1250 ���
			{
				ps3_sprite.setPosition(640, 300); //������ ������� ����� 3 ���������� ������
				num_pre_start = 3; //���������� num_pre_start ��������� �������� 3, ����� � ����� ����� ������� ����������� ������
			}
			if (count_pre_start > 1250 && count_pre_start <= 2500) //���� ���������� �������� ������ 1250 ���, �� ������ 2500 ���
			{
				ps2_sprite.setPosition(640, 300); //������ ������� ����� 2 ���������� ������
				num_pre_start = 2; //���������� num_pre_start ��������� �������� 2, ����� � ����� ����� ������� ����������� ������
			}
			if (count_pre_start > 2500 && count_pre_start <= 3750) //���� ���������� �������� ������ 2500 ���, �� ������ 3750 ���
			{
				ps1_sprite.setPosition(640, 300); //������ ������� ����� 1 ���������� ������
				num_pre_start = 1; //���������� num_pre_start ��������� �������� 1, ����� � ����� ����� ������� ����������� ������
			}
			if (count_pre_start > 3750) //���� ���������� �������� ������ 3750 ���
			{
				num_pre_start = 4; //���������� num_pre_start ��������� �������� 4, ����� � ����� ����� ������� ����������� ������ (�������)
				ps0_sprite.setPosition(210, 300); //������ ������� ��������� �������
				p.set_pre_start(); //� � ��� ����� ������ ���������� pre_start = false, ����� ���� ��������
			}
		}
		/*-----------------------------------------------------------------------------------------*/



		  /*---------------------------------------------------------------------------------------------*/
		 /*--------------------����������� ���������� �������� + �������� ��� ������--------------------*/
		/*---------------------------------------------------------------------------------------------*/
		p.inc_movetimer_health(time); //����������� ���������� movetimer �� time. ����� � ��� ��������� 1250 ��� ��������� ��������
		if (p.get_movetimer_health() >= 1250 && !p.get_pause() && p.get_life()) //���� 1250 ��� ��� ���������� � �� ����� � �������� ���
		{
			p.dec_health(4); //��������� �������� (health) �� 4
			p.zero_movetimer_health(); //�������� ���������� movetimer
		}

		if (!p.get_life()) //���� �������� ����
		{
			//���� �������� ����� ������ menu, ������� ����� ��������� 2 ���������� (minutes, seconds) � �������� ���������
			//� ������ (����� ����, �����)
			m.set_true_check_death();
		}
		/*---------------------------------------------------------------------------------------------*/



		Event event; //������ ������ event
		while (window.pollEvent(event)) //���� ���-�� ��� event
		{
			if (event.type == Event::Closed) //���� event ��������
			{
				//�������� ������������ ������
				for (it = objlist.begin(); it != objlist.end();) //�������� ���� ������ ��������
				{
					entity* b = *it; //������ ��� ��������
					it = objlist.erase(it); delete b; ++it; //������� �� ������ � �� ������
				}
				for (it = entities.begin(); it != entities.end();) //�������� ���� ������ ������
				{
					entity* b = *it; //������ ��� ��������
					it = entities.erase(it); delete b; ++it; //������� �� ������ � �� ������
				}
				for (it = ammolist.begin(); it != ammolist.end();) //�������� ���� ������ ��������
				{
					entity* b = *it; //������ ��� ��������
					it = ammolist.erase(it); delete b; ++it; //������� �� ������ � �� ������
				}
				window.close(); //��������� ����
			}



			/*--------------------------------------------------------------------------------*/
		   /*--------------------���������� ��������, ����� ��������� ���--------------------*/
		  /*--------------------------------------------------------------------------------*/
			if (p.get_buff() == true) //���� �� ��������� ���
			{
				if (p.get_bubbles() > 0) //���� ���-�� �������� > 0
				{
					if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) //���� ������ ������� ������
					{																		//������� ������ (������) � ����� ��������� ������
						if (p.get_check_bubble_fly() == 2) //���� ���� ������� ������
							ammolist.push_back(new cbubble(ammoimage, p.get_sprite().getPosition().x + 60, p.get_sprite().getPosition().y + 13, 16, 16, 0.35, 0, "ammo_right"));
						//������� � ������� � ������ ������ ������ cbubble. ��� �������: ammo_right ����� ��� ����, ����� ����������
						//� ����� ������� ������ ������ (������)
						else //���� ���� ������� �����
							ammolist.push_back(new cbubble(ammoimage, p.get_sprite().getPosition().x, p.get_sprite().getPosition().y + 13, 16, 16, 0.35, 0, "ammo_left"));
						//������� � ������� � ������ ������ ������ cbubble. ��� �������: ammo_left ����� ��� ����, ����� ����������
						//� ����� ������� ������ ������ (�����)
						p.dec_bubbles(); //��������� ���-�� �������� �� -1
					}
				}
				else //���� ������� ������ ���
				{
					p.set_buff_drop(false); //�������, ��� ��� ����� ���������� �� ����� ����� �����
					p.set_buff(false); //� � ��� ��� ���� ���� ���� ����
				}
			}
			/*--------------------------------------------------------------------------------*/



			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) //���� ������ ������� esc (������ �� ����� ��� �������)
			{
				if (!p.get_pre_start() && p.get_life()) //����� �� ����� ������� ����� ������ ���� ���� �������� � ����� ���
				{
					p.set_pause(); //������ �������� ���� pause (true ��� false)
					m.swap_check_pause();
				}
			}
		}

		/*------------------------------------------------------------------------*/
	   /*--------------------���������� ������� ���� �� �����--------------------*/
	  /*------------------------------------------------------------------------*/
		if (!p.get_buff()) //���� ���� � ��� ��� ���� ����
		{
			p.inc_movetimer_buff(time); //�������� ������ �� ��������� ����
		}
		if (p.get_movetimer_buff() > 77500 && !p.get_buff_drop()) //���� ������ 77500 ��� (~ 60 �) � ��� �� �� �����, �� ������� ���
		{
			objlist.push_back(new object(buffimage, p.generatorhavki(), 64, 58, 58, 0, 0.25, "buff_1")); //������� �������� �� X ����� ������ (���)
			p.set_buff_drop(true); //�������, ��� ��� ����� �� �����
		}
		/*------------------------------------------------------------------------*/



		  /*--------------------------------------------------------------------------------------------*/
		 /*--------------------�������� �� ����������� � ��������� (������ objlist)--------------------*/
		/*--------------------------------------------------------------------------------------------*/
		for (it = objlist.begin(); it != objlist.end();) //��������� ������ ��������
		{
			entity* b = *it; //������, ����� ������ ��� ������ �� (*it)->, � b->
			b->update(time); //�������� �-�� update ��� ���� ��������� ������ objlist (�������� ������ object)
			if (b->get_eaten() == true) { it = objlist.erase(it); delete b; } //���������, ���� �������������� � �������� (eaten == true), ��
			else it++; //������� �� ������ � �� ������������ ������, � ��������� ����� ���� ������ �� ������
		}
		for (it = objlist.begin(); it != objlist.end(); it++) //��������� ������ �������� (objlist) �� ������ �� �����
		{
			if ((*it)->getrect().intersects(p.getrect())) //���� ������ ������� ������������� �� �������� ���������
			{
				if ((*it)->get_name() == "eat_1") { p.inc_health(16); p.inc_count_get_eat(); } //���� ���, ���������� 16 �� (������� ���)
				(*it)->set_eaten(true); //��������, ��� ��� �������, ����� � ��������� ��� ��� ��������� (��� ��� ��������)
				if ((*it)->get_name() == "buff_1") //���� ���
				{
					p.set_buff(true); //�� ������ � ��� ����
					p.set_bubbles(); //������ 2 �������
					p.set_movetimer_buff(0); //�������� ������� ��������� ����
					p.inc_count_get_buff(); //������� ����������� �����
				}
			}
		}
		/*--------------------------------------------------------------------------------------------*/



		  /*----------------------------------------------------------------------------------------------*/
		 /*--------------------���������� �������� ������� ������ (����� �����������)--------------------*/
		/*----------------------------------------------------------------------------------------------*/
		for (it = ammolist.begin(); it != ammolist.end();) //���������� �� ������ ��������
		{
			entity* b = *it; //��� ��������
			if (b->get_name() == "ammo_right") //���� ����� �������� ������ (�� ����� �������� ����� ��� �������� ������� ������ cbubble)
			{
				b->update(time); //������� ������ ������ (�������� �� X �������������)
			}
			else //�����, ���� ��� ������� ammo_left (�� ���� ����� �������� �����)
			{
				b->update(-time); //������� ������ ����� (�������� �� X �������������)
			}
			if (b->get_bubble_crash()) { it = ammolist.erase(it); delete b; b->set_bubble_crash(false); }
			//���� ������ �������� � �����, ������� ��� �� ������ � �� ������, � ��� �� �������� �� ������������ ������ false
			else it++; //����� ������ ������ �� ������ ��������
		}
		/*----------------------------------------------------------------------------------------------*/



		  /*-----------------------------------------------------------------------------------------------------*/
		 /*--------------------�������� ������ objlist �� ��, ������� � ����� �������� � ���--------------------*/
		/*-----------------------------------------------------------------------------------------------------*/
		if (objlist.size() < 3) //���� �������� ��� � ������ ������ 3-�
		{
			objlist.push_back(new object(eatimage, p.generatorhavki(), 64, 20, 22, 0, 0.25, "eat_1")); //������� ��� � ������� � ����� ������
		}
		else //����� (���� � ������ >= 3-� ��������)
		{
			if (objlist.size() == 3) //���� �������� � ������ ����� 3
			{
				for (it = objlist.begin(); it != objlist.end(); it++) //��������� ������ ��������
				{
					if ((*it)->get_name() != "eat_1") //���������: ���� ���� �� ���� ������ �� ����� ��� eat_1 (������ �� ������ 2 ��� � ���)
					{
						objlist.push_back(new object(eatimage, p.generatorhavki(), 64, 20, 22, 0, 0.25, "eat_1")); //������� ��� � ������� � ����� ������
					}
				}
			}
		}
		/*-----------------------------------------------------------------------------------------------------*/



		  /*---------------------------------------------------------------------------------------*/
		 /*--------------------�������������� ���� �� ��������� � � ����������--------------------*/
		/*---------------------------------------------------------------------------------------*/
		for (it = entities.begin(); it != entities.end(); ++it) //��������� ������ ������ (entities) �� ������ �� �����
		{
			for (it_2 = ammolist.begin(); it_2 != ammolist.end(); ++it_2) //��������� ������ �������� (ammolist)
			{
				entity* b = *it_2; //������ ��� ��������
				if ((*it)->getrect().intersects(b->getrect())) //���� ������ ����� ������������� �� �������� �������
				{
					(*it)->set_stun(true); //����� ��������
					b->set_bubble_crash(true); //��������, ��� ���������, ����� ������ �������� �� ������ � ������
					p.inc_count_stun_shark(); //������� ���������� ��������� ����
				}
			}
			for (it_2 = ammolist.begin(); it_2 != ammolist.end();) //��������� ������ �������� (ammolist)
			{
				entity* q = *it_2;
				if (q->get_bubble_crash() == true) { it_2 = ammolist.erase(it_2); delete q; q->set_bubble_crash(false); }
				//���� ������ ��������, ������� ��� �� ������ � ������, � ��� �� ������ �������� ������������ (bubble_crash = falses)
				else it_2++; //����� ���� ������ �� ������
			}
			(*it)->inc_movetimer_stun(time); //�������� ������ ��������
			if ((*it)->get_stun()) //���� ����� ��������
			{
				if ((*it)->get_movetimer_stun() < 3750) //���� 3 ������� ��������� ��� �� ������
				{
					(*it)->update(0); //����� �����������. �� �������� � ������ update ���������� �� �� ����� sfml, � �� 0, � ������ = 0
					if ((*it)->get_check_shark_stun() == 1) //���� 1, ������ ����� ����� �����
					{
						(*it)->set_sprite(126, 48, -126, -48); //�������������� ������ ����� � ����� ������ :)
					}
					else //�����, ���� 2 (����� ���� ������ 1 ��� 2), ������ ����� ����� ������
					{
						(*it)->set_sprite(0, 48, 126, -48); //�������������� ������ ����� � ����� ������ :)
					}
				}
				else //�����, ���� 3 ������� ��� ������
				{
					(*it)->set_movetimer_stun(0); //���������� �������
					(*it)->set_stun(false); //�������, ��� ����� ������ � �������
					if ((*it)->get_check_shark_stun() == 1) //���� 1, ������ ����� ����� �����
					{
						(*it)->set_sprite(126, 0, -126, 48); //���������� ����� ���������� ��������� �������
					}
					else //�����, ���� 2 (����� ���� ������ 1 ��� 2), ������ ����� ����� ������
					{
						(*it)->set_sprite(0, 0, 126, 48); //���������� ����� ���������� ��������� �������
					}
				}
			}
			else //����� (���� ����� �� ��������)
			{
				(*it)->update(time); //�������� ����� update ��� ������� ������� ������ entities (��� ������� �����), ���� �� �� �������
				(*it)->set_movetimer_stun(0); //���� ����� �� ��������, ���������� �������
			}
			if (!p.get_kusy()) //���� �� �� ��������� ��� ������������� �� ����
			{
				if ((*it)->getrect().intersects(p.getrect()) && (*it)->get_stun() == false && p.get_life()) //���� ������ ����� ������������ � ����������
				{																							//� ����� �� �������� � �������� ���
					p.dec_health(25); // -25 ��
					p.set_kusy(true); //����!
				}
			}
		}
		/*---------------------------------------------------------------------------------------*/



		if (m.get_check_restart()) //���� ������ ������� "����� ����" �� ����� �����
		{
			return true; //��������� �-��, ��������� true ��� �������� �������� � ���������� ������ ���� �-�� is_game (���������� ����� ����)
		}

		p.update(time); //����� update ������ player
		hp.update(p.get_health(), p.get_sprite().getPosition().x, p.get_sprite().getPosition().y); //����� update ��� ����� ��������

		window.setView(view); //������������� ������ � ������ ����� (�������� �� 64 ������� ���� � ������, ����� �� ���� ����� ������ �����)

		window.clear(); //�������� ���� �� ����������� �����

		p.draw_map(window, s_map);

		ostringstream playerhealth, minutesstring, secondsstring, counteat, countbuff, countstun;
		//������� ����������� ������ ��� ������ �� �� ����� (������� ��������� � �������) � �������� � ��� ������� ������
		playerhealth << p.get_health(); //������ ��������� ������ �������� (������ ��� ������)
		minutesstring << minutes; //������ �������� ����� ����
		secondsstring << seconds; //������ �������� ������ ���� (�� 60 ������)
		counteat << p.get_count_get_eat(); //������ �������� ��������� ���
		countbuff << p.get_count_get_buff(); //������ �������� ����������� �����
		countstun << p.get_count_stun_shark(); //������ �������� ��������� ����
		if (p.get_life()) //���� �������� ���
		{
			text.setString(/*L"��������: " + playerhealth.str() + */L"Game time: " + minutesstring.str() + L" minutes " + secondsstring.str() + L" seconds "); //������� �� �����
			text.setPosition(1550, 64); //������ ������� ������ �� ������
			window.draw(text); //������ �����
		}

		for (it = objlist.begin(); it != objlist.end(); ++it) { window.draw((*it)->get_sprite()); } //������ ���
		for (it = entities.begin(); it != entities.end(); ++it) { window.draw((*it)->get_sprite()); } //������ ������
		for (it = ammolist.begin(); it != ammolist.end(); ++it) { window.draw((*it)->get_sprite()); } //������ ������


		window.draw(hp.get_sprite_hp()); //������ ������� �������� ��� ����������
		window.draw(hp.get_black_bar()); //������ ������ ������� ������ ��������

		if (p.get_bubbles() == 2) //���� � ��� 2 ������ ���������
		{
			ammo_count_sprite.setPosition(p.get_sprite().getPosition().x + 12, p.get_sprite().getPosition().y + 46); //������������� ������
			window.draw(ammo_count_sprite); //������ ������ ������
			ammo_count_sprite.setPosition(p.get_sprite().getPosition().x + 32, p.get_sprite().getPosition().y + 46); //������������� ������
			window.draw(ammo_count_sprite); //������ ������ ������
		}
		if (p.get_bubbles() == 1) //���� � ��� 1 ����� ��������
		{
			ammo_count_sprite.setPosition(p.get_sprite().getPosition().x + 12, p.get_sprite().getPosition().y + 46); //������������� ������
			window.draw(ammo_count_sprite); //������ ������ ������
		}
		window.draw(p.get_sprite()); //������ ���������

		if (num_pre_start == 3) //�������� �� ��, ����� ����� �������� �� ����� �� ������ ���� (3)
			window.draw(ps3_sprite); //���������� ������ ��� ����� (3)
		if (num_pre_start == 2) //�������� �� ��, ����� ����� �������� �� ����� �� ������ ���� (2)
			window.draw(ps2_sprite); //���������� ������ ��� ����� (2)
		if (num_pre_start == 1) //�������� �� ��, ����� ����� �������� �� ����� �� ������ ���� (1)
			window.draw(ps1_sprite); //���������� ������ ��� ����� (1)
		if (num_pre_start == 4) //�������� �� ��, ����� ����� �������� �� ����� �� ������ ���� (�������)
		{
			//����� ����� ������� �� ��������� ��������� ��� �������� ������� ������������ ������������ ������� (���������� ������������)
			if (count_pre_start > 3750 && count_pre_start <= 4000) //���� ���������� count_pre_start �������� > 3750 ���, �� <= 4000 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 255)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 4000 && count_pre_start <= 4250) //���� ���������� count_pre_start �������� > 4000 ���, �� <= 4250 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 220)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 4250 && count_pre_start <= 4500) //���� ���������� count_pre_start �������� > 4250 ���, �� <= 4500 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 190)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 4500 && count_pre_start <= 4750) //���� ���������� count_pre_start �������� > 4500 ���, �� <= 4750 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 160)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 4750 && count_pre_start <= 5000) //���� ���������� count_pre_start �������� > 4750 ���, �� <= 5000 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 130)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 5000 && count_pre_start <= 5250) //���� ���������� count_pre_start �������� > 5000 ���, �� <= 5250 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 100)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 5250 && count_pre_start <= 5500) //���� ���������� count_pre_start �������� > 5250 ���, �� <= 5500 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 70)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 5500 && count_pre_start <= 5750) //���� ���������� count_pre_start �������� > 5500 ���, �� <= 5750 ���
			{
				ps0_sprite.setColor(Color(255, 255, 255, 40)); //������ ���� � ������������ ������� � �������� (R, G, B, �����-��)
				window.draw(ps0_sprite); //������ ��� �������
			}
			if (count_pre_start > 5750) //���� ���������� count_pre_start �������� > 5750 ���
			{
				num_pre_start = 0; //��������, ��� num_pre_start, ����� ���������� �� ��, ����� �������� �������� �� ������ ���� = 0
			}
		}
		if (num_pre_start == 0) //���� ������� �������� ��� �������� �� �����
			count_pre_start = 0; //�������� ������� �������, ���������� �� �������� ����� �������� �� ������ ����, ����� �� �� ��� �� ����� ����

		if (p.get_pause() || !p.get_life()) //���� ����� ����� ��� ���� �� ����
		{
			//���� �������� ����� ������ menu, �������� ���������� �� true, �� ���� ���� ����� ���������� ������ ���� �� ������� ������
			m.pause_func(window); //�������� �-�� ��� ���������� ������ ���� ����� (��� ������ �� �� ����� ������)
			window.draw(m.get_pause_sprite_1()); //������ ������ ������
			window.draw(m.get_pause_sprite_2()); //������ ������ ������
		}

		if (!p.get_life()) //���� ����� ���� ������� ����������
		{
			text.setString(L"Your result: " + minutesstring.str() + L" minutes " + secondsstring.str() + L" seconds " + L"\nFood taken: " + counteat.str() + L"\nBuff taken: " + countbuff.str() + L"\nStunned sharks: " + countstun.str()); //������� �� �����
			text.setPosition(598, 440); //������ ������� ������ �� ������
			window.draw(text); //������ �����
		}

		window.display(); //���������� ��� ��� ����� � ��� ������
	}
	return false;
}

void restart_func(RenderWindow& window) //������������� �-�� ��� �������� �������� (��� ����������� ����������� ����)
{
	if (is_game(window)) //���� �������� �-�� is_game ���������� true (� ������ ������� ������ ����� ����)
		restart_func(window); //�������� �-�� restart_func, ������� � ���� ������� ����� ������� is_game
}