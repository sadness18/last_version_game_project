#include <iostream> //что-то
#include <sstream> //еще что-то
#include <list> //подключаем списки библиотеки STL
#include <SFML/Graphics.hpp> //подключаем графику sfml
#include <ctime> //подключаем время
#include <math.h> //библиотека с математическими функциями
#include "map.h" //подключаем файл с картой
#include "life_bar.h" //подключаем файл с классом, отвечающим за реализацию шкалы здоровья
#include "menu.h"
#include "class.h"

using namespace std; //пространство имен std, чтобы не писать каждый раз std::
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

player::player(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name) //конструктор
{
	sprite.setTextureRect(IntRect(0, 0, w, h));
}

void player::control(float time, float Dx, float Dy) //перемещение персонажа
{
	x += Dx * time;
	y += Dy * time;
	speed = 0;
	sprite.setPosition(x, y);
	physical_obj(Dx, Dy);
}

void player::physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
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
	randomx = 84 + rand() % 1836; //генератор еды. Числа такие, потому что нужно исключить вероятность краев
	std::cout << randomx << std::endl;	 // и -20 пикселей слева и справа от краев видимой области (проще говоря FHD экран 1920 по иксу
	return randomx;				//  а еда генерируется в диапазоне с 84-го по 1836 пиксель включительно)
}

void player::update(float time)
{
	if (life) //если игрок жив (life = true), то он может двигаться
	{
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) //движение влево (кнопка A)
		{
			sprite.setTextureRect(IntRect(60, 0, -60, 42)); //повернуть спрайт рыбы влево
			check_bubble_fly = 1; //это нужно для того, чтобы при стрельбе пузырями проверять, куда повернута рыба
			speed = 0.15; //скорость движения
			dx = -speed; dy = 0; //скорость по координатам
			control(time, dx, dy); //вызываем ф-ию control и передаем ей время и скорость по координатам
		}
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) //движение вправо (кнопка D)
		{
			sprite.setTextureRect(IntRect(0, 0, 60, 42)); //повернуть спрайт рыбы вправо
			check_bubble_fly = 2; //это нужно для того, чтобы при стрельбе пузырями проверять, куда повернута рыба
			speed = 0.15; //скорость движения
			dx = speed; dy = 0; //скорость по координатам
			control(time, dx, dy); //вызываем ф-ию control и передаем ей время и скорость по координатам
		}
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) //движение вверх (кнопка W)
		{
			speed = 0.15; //скорость движения
			dx = 0; dy = -speed; //скорость по координатам
			control(time, dx, dy); //вызываем ф-ию control и передаем ей время и скорость по координатам
		}
		if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) //движение вниз (кнопка S)
		{
			speed = 0.15; //скорость движения
			dx = 0; dy = speed; //скорость по координатам
			control(time, dx, dy); //вызываем ф-ию control и передаем ей время и скорость по координатам
		}
	}
	if (health <= 0) { life = false; } //если здоровье <= 0, то смерть
	if (health > 100) health = 100; //если здоровье переваливает за 100 делаем здоровье = 100, ибо больше 100 быть не должно
	if (!life) health = 0; //для того, чтобы хп в минус не уходили (если мертв, хп = 0)

	sweem += time; //переменная и условия для эффекта плавания
	if (sweem > 150) sprite.setPosition(x, y + 3);
	if (sweem > 300) sprite.setPosition(x, y + 3);
	if (sweem > 450) sprite.setPosition(x, y - 3);
	if (sweem > 600) { sprite.setPosition(x, y - 3); sweem = 0; }

	movetimer += time; //счетчик времени (начинает накапливать в себе 2 секунды, если персонаж соприкоснулся с акулой)
	if (kusy == true) //если все-таки кусь
	{
		for (int i = 1; i <= 10; i++) //считаем от 1 до 10 включительно (для эффекта моргания неуязвимости)
		{
			if (i % 2 != 0) //если нечетная итерация
			{
				if (movetimer > 200 * (i - 1) && movetimer < 200 * i) { sprite.setColor(Color::Transparent); } //спрайт прозрачный
			}
			else //иначе, если четная
			{
				if (movetimer > 200 * (i - 1) && movetimer < 200 * i) { sprite.setColor(Color::White); } //спрайт обычный
			}
		}
		if (movetimer > 2000) //если уже прошло 2 секунды
		{
			sprite.setColor(Color::White); //делаем спрайт обычным
			kusy = false; //кусь проходит, боль уходит
			movetimer = 0; //обнуляем счетчик
		}
	}
	else movetimer = 0; //если не кусь, то и зачем считать тогда
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
	for (int i = 0; i < height_map; i++) //циклы для прорисовки карты
	{
		for (int j = 0; j < width_map; j++)
		{
			if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 64, 64)); //обычная вода
			if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 128, 64)); //водоросли(препятствие)
			if (TileMap[i][j] == 'd') s_map.setTextureRect(IntRect(128, 0, 192, 64)); //грязь внизу (препятствие)
			if (TileMap[i][j] == 'k') s_map.setTextureRect(IntRect(192, 0, 256, 64)); //осьминог (декор)
			if (TileMap[i][j] == 'c') s_map.setTextureRect(IntRect(0, 64, 64, 128)); //краб (декор)
			if (TileMap[i][j] == 'y') s_map.setTextureRect(IntRect(64, 64, 128, 128)); //утка (декор)
			if (TileMap[i][j] == 'u') s_map.setTextureRect(IntRect(128, 64, 192, 128)); //улитка горизонтальная (декор)
			if (TileMap[i][j] == 'U') s_map.setTextureRect(IntRect(192, 64, 256, 128)); //улитка вертикальная (декор)
			if (TileMap[i][j] == '1') s_map.setTextureRect(IntRect(0, 128, 64, 192)); //папоротник средний (декор)
			if (TileMap[i][j] == '2') s_map.setTextureRect(IntRect(64, 128, 128, 192)); //папоротник жирный (декор)
			if (TileMap[i][j] == '3') s_map.setTextureRect(IntRect(128, 128, 192, 192)); //папоротник тонкий (декор)
			if (TileMap[i][j] == '4') s_map.setTextureRect(IntRect(192, 128, 256, 192)); //папоротник кривой (декор)

			s_map.setPosition(j * 64, i * 64); //расставляем блоки текстур
			window.draw(s_map); //рисуем карту
		}
	}
}

//////////ENEMY//////////

enemy::enemy(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name) //конструктор
{
	sprite.setTextureRect(IntRect(0, 0, w, h));
	//dx = 0.2; //начальная скорость
}

void enemy::physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
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
					x = j * 64 - w; dx = -Dx; //если акула врезалась в препятствие, то разворачиваем ее (влево)
					sprite.setTextureRect(IntRect(126, 0, -126, 48)); //разворачиваем спрайт влево
					check_shark_stun = 1; //для проверки того, в какую сторону будет лежать оглушенная акула
				}
				if (Dx < 0)
				{
					x = j * 64 + 64; dx = -Dx; //если акула врезалась в препятствие, то разворачиваем ее (вправо)
					sprite.setTextureRect(IntRect(0, 0, 126, 48));//разворачиваем спрайт вправо
					check_shark_stun = 2; //для проверки того, в какую сторону будет лежать оглушенная акула
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

	if (name == "enemy_1" || name == "enemy_2") //для акул №1 и №2
	{
		sweem += time; //переменная и условия для эффекта плавания
		if (sweem > 100) sprite.setPosition(x, y + 3); //+150
		if (sweem > 250) sprite.setPosition(x, y + 3); //+300
		if (sweem > 550) sprite.setPosition(x, y - 3); //+450
		if (sweem > 1000) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
	if (name == "enemy_3" || name == "enemy_7") //для акул №3 и №7
	{
		sweem += time; //переменная и условия для эффекта плавания
		if (sweem > 100) sprite.setPosition(x, y + 3); //+100
		if (sweem > 200) sprite.setPosition(x, y + 3); //+100
		if (sweem > 300) sprite.setPosition(x, y - 3); //+100
		if (sweem > 400) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
	if (name == "enemy_5" || name == "enemy_9") //для акул №5 и №9
	{
		sweem += time; //переменная и условия для эффекта плавания
		if (sweem > 220) sprite.setPosition(x, y + 3); //+100
		if (sweem > 320) sprite.setPosition(x, y + 3); //+240
		if (sweem > 560) sprite.setPosition(x, y - 3); //+340
		if (sweem > 900) { sprite.setPosition(x, y - 3); sweem = 0; }
	}
	if (name == "enemy_4" || name == "enemy_6" || name == "enemy_8" || name == "enemy_10") //для акул №4, №6, №8 и №10
	{
		sweem += time; //переменная и условия для эффекта плавания
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
	//dy = 0.25; //начальная скорость
}

void object::physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
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
	//dx = 0.35; //скорость полета снаряда
}

void cbubble::physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
{
	for (int i = y / 64; i < (y + h) / 64; i++)
	{
		for (int j = x / 64; j < (x + w) / 64; j++)
		{
			if (TileMap[i][j] == '0' || TileMap[i][j] == 'd')
			{
				/*if (Dy > 0) y = i * 64 - h;
				if (Dy < 0) y = i * 64 + 64;*/
				if (Dx > 0) //если врезается слева
				{
					x = j * 64 - w; //останавливается
					bubble_crash = true; //сообщаем, что врезались
				}
				if (Dx < 0) //если врезается справа
				{
					x = j * 64 + 64; //останавливается
					bubble_crash = true; //сообщаем, что врезались
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

bool is_game(RenderWindow& window) //ф-ия is_game, принимающая в качестве параметра ссылку на объект окно из ф-ии main. Там же она и вызывается
{
	setlocale(LC_ALL, "rus");
	View view; //объект камера.
	view.reset(FloatRect(64, 64, 1920, 1080)); //инициализация камеры. Поставить камеру, начиная с точки 64, 64, размер камеры 1920х1080

	Font font; //объект шрифт
	font.loadFromFile("18386.ttf"); //выбираем файл шрифта
	Text text("", font, 20); //объект текст (строка, шрифт, размер)
	text.setOutlineColor(Color::Black); //задать цвет тексту (внешняя линия)
	text.setFillColor(Color::Black); //задать цвет тексту (заливка)

	Image map_image; //объект изображение (для карты)
	map_image.loadFromFile("images/map.png"); //загружаем картинку элементов карты
	Texture t_map; //объект текстура
	t_map.loadFromImage(map_image); //загружаем изображение в текстуру
	Sprite s_map; //объект спрайт
	s_map.setTexture(t_map); //загружаем текстуру в спрайт

	Image heroimage; //объект изображение (для игрока)
	heroimage.loadFromFile("images/fish.png"); //выбираем изображение для игрока

	Image enemyoneimage; //объект изображение (для врага)
	enemyoneimage.loadFromFile("images/shark.png"); //выбираем изображение для врага

	Image eatimage; //объект изображение (для еды)
	eatimage.loadFromFile("images/eat.png"); //выбираем изображение для еды

	Image buffimage; //объект изображение (для бафа)
	buffimage.loadFromFile("images/bubble_bonus.png"); //выбираем изображение для бафа

	Image ammoimage; //объект изображение (для снарядов пузырей)
	ammoimage.loadFromFile("images/bubble_shot_2.png"); //выбираем изображение для снаряда пузыря
	Texture ammo_count_texture; //задаем текстуру для кол-ва снарядов у нас (под рыбой)
	ammo_count_texture.loadFromImage(ammoimage); //выбираем изображение (такое же, как и при самом выстреле)
	Sprite ammo_count_sprite; //создаем спрайт, чтобы отображать кол-во имеющихся у нас снарядок
	ammo_count_sprite.setTexture(ammo_count_texture); //помещаем в спрайт текстуру снарядов

	Texture ps3_texture; //объект класса текстура для цифры 3 перед началом игры
	ps3_texture.loadFromFile("images/ps_3.png"); //задаем сам файл изображения для цифры 3
	Sprite ps3_sprite; //объект класса спрайт для цифры 3
	ps3_sprite.setTexture(ps3_texture); //задаем текстуру спрайту

	Texture ps2_texture; //объект класса текстура для цифры 2 перед началом игры
	ps2_texture.loadFromFile("images/ps_2.png"); //задаем сам файл изображения для цифры 2
	Sprite ps2_sprite; //объект класса спрайт для цифры 2
	ps2_sprite.setTexture(ps2_texture); //задаем текстуру спрайту

	Texture ps1_texture; //объект класса текстура для цифры 1 перед началом игры
	ps1_texture.loadFromFile("images/ps_1.png"); //задаем сам файл изображения для цифры 1
	Sprite ps1_sprite; //объект класса спрайт для цифры 1
	ps1_sprite.setTexture(ps1_texture); //задаем текстуру спрайту

	Texture ps0_texture; //объект класса текстура для надписи перед началом игры
	ps0_texture.loadFromFile("images/ps_0.png"); //задаем сам файл изображения для надписи
	Sprite ps0_sprite; //объект класса спрайт для надписи
	ps0_sprite.setTexture(ps0_texture); //задаем текстуру спрайту

	list<entity*> entities; //динамический список для помещения туда врагов
	list<entity*>::iterator it; //итератор для этого списка
	list<entity*>::iterator it_2; //второй итератор. Нужен для того, чтобы прогонять 2 разных списка одновременно (для проверки оглушения)

	entities.push_back(new enemy(enemyoneimage, 1856, 64, 126, 48, 0.3, 0, "enemy_1")); //враг 1
	entities.push_back(new enemy(enemyoneimage, 64, 128, 126, 48, 0.3, 0, "enemy_2")); //враг 2
	entities.push_back(new enemy(enemyoneimage, 64, 320, 126, 48, 0.22, 0, "enemy_3")); //враг 3
	entities.push_back(new enemy(enemyoneimage, 320, 576, 126, 48, 0.2, 0, "enemy_4")); //враг 4
	entities.push_back(new enemy(enemyoneimage, 64, 832, 126, 48, 0.27, 0, "enemy_5")); //враг 5
	entities.push_back(new enemy(enemyoneimage, 768, 704, 126, 48, 0.24, 0, "enemy_6")); //враг 6
	entities.push_back(new enemy(enemyoneimage, 1344, 256, 126, 48, 0.22, 0, "enemy_7")); //враг 7
	entities.push_back(new enemy(enemyoneimage, 1152, 384, 126, 48, 0.22, 0, "enemy_8")); //враг 8
	entities.push_back(new enemy(enemyoneimage, 1856, 512, 126, 48, 0.22, 0, "enemy_9")); //враг 9
	entities.push_back(new enemy(enemyoneimage, 1600, 768, 126, 48, 0.2, 0, "enemy_10")); //враг 10

	player p(heroimage, 64, 1024, 60, 42, 0.15, 0, "player_1"); //создаем персонажа (объект подкласса player)
	life_bar hp; //объект класса life_bar для отображения полосы здоровья

	srand(time(0)); //псевдорандом (сброс предыдущего сгенерированного числа)

	list<entity*> objlist; //список, хранящий в себе ссылки на динамические объекты класса object (еда, баф)
	for (int i = 0; i <= 2; i++) //первоначальная генерация калорий (3 шт.)
	{
		objlist.push_back(new object(eatimage, p.generatorhavki(), 64, 20, 22, 0, 0.25, "eat_1")); //создаем заносим в список object 3 элемента (3 еды)
	}

	list<entity*> ammolist; //список, хранящий заряды пузырей

	menu m; //объект класса menu

	Clock clock; //переменная времени
	int minutes = 0; //кол-во минут, прошедших с момента начала игры
	int seconds = 0; //кол-во секунд, которые переводятся в минуты, когда достигают 60
	int save_time = 0; //для сохранения времени
	int count_pre_start = 0; //для накопления времени перед началом игры
	int num_pre_start = 0; //номер картинки, которую необходимо выводить на экран. Значение задается вначале цикла, а проверяется в конце

	  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 ////////////////////////////////////////////////////ПОКА ОТКРЫТО ОКНО////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen()) //цикл "пока открыто окно". Все, что в нем написано, происходит с постоянным обновлением (каждый фрейм (тик))
	{
		float time; //переменная для занесения в нее времени, для последующей реализации передвижения спрайтов (т.н. привязка ко времени)



		  /*------------------------------------------------------------------------------------------------*/
		 /*--------------------Реализация времени для движения и остановки (если пауза)--------------------*/
		/*------------------------------------------------------------------------------------------------*/
		if (!p.get_pause()) //Если пауза НЕ стоит
			time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		else //если пауза стоит
			time = 0; //обнуляем время, чтобы ничего не двигалось на экране
		/*------------------------------------------------------------------------------------------------*/



		clock.restart(); //перезагружает время (обнуляет)
		time = time / 800; //скорость игры. Для справки (напоминалка) --> 1 000 000 мкс (1 с) / 800 = 1250 мкс (1 с)



		  /*------------------------------------------------------------------------*/
		 /*--------------------Реализация подсчета времени игры--------------------*/
		/*------------------------------------------------------------------------*/
		if (p.get_life() && !p.get_pause() && !p.get_pre_start()) //если выполняются 3 условия сразу: игрок жив, пауза НЕ стоит и игра началась
		{
			save_time += time; //в переменной save_time накапливаем микросекунды
			if (save_time >= 1250) //если накопилось 1250 мкс или больше (1 секунда)
			{
				p.inc_count_gametime(); //инкрементируем переменную count_gametime. Она и накапливает в себе игровое время
				save_time = 0; //обнуляем save_time, чтобы снова накапливать в ней 1250 мкс. И так постоянно, пока выполняются условия
			}
		}
		minutes = floor(p.get_count_gametime() / 60); //для вывода на экран времени в минутах (сам вывод в конце, где отрисовка объектов)
		seconds = p.get_count_gametime() % 60; //для вывода на экран времени в секундах (до 60) (сам вывод в конце, где отрисовка объектов)
		/*------------------------------------------------------------------------*/



		  /*-----------------------------------------------------------------------------------------*/
		 /*--------------------Реализация 3-х секундной подготовки перед началом--------------------*/
		/*-----------------------------------------------------------------------------------------*/
		count_pre_start += time; //накапливаем время в переменной count_pre_start, чтобы в последующем вести отсчет до начала игры
		if (p.get_pre_start()) //проверяем переменную pre_start на значение true
		{
			time = 0; //обнуляем время, чтобы никто и ничто не двигалось, пока отсчет не закончится и игра не начнется
			if (count_pre_start <= 1250) //если переменная count_pre_start еще не накопила в себе 1250 мкс
			{
				ps3_sprite.setPosition(640, 300); //задаем позицию цифры 3 посередине экрана
				num_pre_start = 3; //переменная num_pre_start принимает значение 3, чтобы в самом конце вывести необходимый спрайт
			}
			if (count_pre_start > 1250 && count_pre_start <= 2500) //если переменная накопила больше 1250 мкс, но меньше 2500 мкс
			{
				ps2_sprite.setPosition(640, 300); //задаем позицию цифры 2 посередине экрана
				num_pre_start = 2; //переменная num_pre_start принимает значение 2, чтобы в самом конце вывести необходимый спрайт
			}
			if (count_pre_start > 2500 && count_pre_start <= 3750) //если переменная накопила больше 2500 мкс, но меньше 3750 мкс
			{
				ps1_sprite.setPosition(640, 300); //задаем позицию цифры 1 посередине экрана
				num_pre_start = 1; //переменная num_pre_start принимает значение 1, чтобы в самом конце вывести необходимый спрайт
			}
			if (count_pre_start > 3750) //если переменная накопила больше 3750 мкс
			{
				num_pre_start = 4; //переменная num_pre_start принимает значение 4, чтобы в самом конце вывести необходимый спрайт (надпись)
				ps0_sprite.setPosition(210, 300); //задаем позицию финальной надписи
				p.set_pre_start(); //и в это время делаем переменную pre_start = false, чтобы игра началась
			}
		}
		/*-----------------------------------------------------------------------------------------*/



		  /*---------------------------------------------------------------------------------------------*/
		 /*--------------------Посекундное уменьшение здоровья + действия при смерти--------------------*/
		/*---------------------------------------------------------------------------------------------*/
		p.inc_movetimer_health(time); //увеличиваем переменную movetimer на time. Когда в ней накопится 1250 мкс уменьшаем здоровье
		if (p.get_movetimer_health() >= 1250 && !p.get_pause() && p.get_life()) //если 1250 мкс уже накопилось и НЕ пауза и персонаж жив
		{
			p.dec_health(4); //уменьшаем здоровье (health) на 4
			p.zero_movetimer_health(); //обнуляем переменную movetimer
		}

		if (!p.get_life()) //если персонаж умер
		{
			//сюда вставить метод класса menu, который будет принимать 2 переменные (minutes, seconds) и выводить результат
			//и кнопки (новая игра, выход)
			m.set_true_check_death();
		}
		/*---------------------------------------------------------------------------------------------*/



		Event event; //объект класса event
		while (window.pollEvent(event)) //пока что-то там event
		{
			if (event.type == Event::Closed) //если event закрылся
			{
				//отчистка динамической памяти
				for (it = objlist.begin(); it != objlist.end();) //отчищаем весь список объектов
				{
					entity* b = *it; //просто для удобства
					it = objlist.erase(it); delete b; ++it; //удаляем из списка и из памяти
				}
				for (it = entities.begin(); it != entities.end();) //отчищаем весь список врагов
				{
					entity* b = *it; //просто для удобства
					it = entities.erase(it); delete b; ++it; //удаляем из списка и из памяти
				}
				for (it = ammolist.begin(); it != ammolist.end();) //отчищаем весь список снарядов
				{
					entity* b = *it; //просто для удобства
					it = ammolist.erase(it); delete b; ++it; //удаляем из списка и из памяти
				}
				window.close(); //закрываем окно
			}



			/*--------------------------------------------------------------------------------*/
		   /*--------------------Реализация стрельбы, когда подобрали баф--------------------*/
		  /*--------------------------------------------------------------------------------*/
			if (p.get_buff() == true) //если мы подобрали баф
			{
				if (p.get_bubbles() > 0) //если кол-во снарядов > 0
				{
					if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) //если нажата клавиша пробел
					{																		//создаем объект (снаряд) в месте координат игрока
						if (p.get_check_bubble_fly() == 2) //если рыба смотрит вправо
							ammolist.push_back(new cbubble(ammoimage, p.get_sprite().getPosition().x + 60, p.get_sprite().getPosition().y + 13, 16, 16, 0.35, 0, "ammo_right"));
						//создать и занести в список объект класса cbubble. Имя объекта: ammo_right нужно для того, чтобы определить
						//в какую сторону лететь пузырю (вправо)
						else //если рыба смотрит влево
							ammolist.push_back(new cbubble(ammoimage, p.get_sprite().getPosition().x, p.get_sprite().getPosition().y + 13, 16, 16, 0.35, 0, "ammo_left"));
						//создать и занести в список объект класса cbubble. Имя объекта: ammo_left нужно для того, чтобы определить
						//в какую сторону лететь пузырю (влево)
						p.dec_bubbles(); //уменьшаем кол-во снарядов на -1
					}
				}
				else //если зарядов больше нет
				{
					p.set_buff_drop(false); //говорим, что баф можно выкидывать на карту через время
					p.set_buff(false); //и у нас при себе тоже бафа нету
				}
			}
			/*--------------------------------------------------------------------------------*/



			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) //если нажата клавиша esc (ставим на паузу так сказать)
			{
				if (!p.get_pre_start() && p.get_life()) //чтобы мы могли ставить паузу только если игра началась и игрок жив
				{
					p.set_pause(); //меняем значение поля pause (true или false)
					m.swap_check_pause();
				}
			}
		}

		/*------------------------------------------------------------------------*/
	   /*--------------------Реализация выброса бафа на карту--------------------*/
	  /*------------------------------------------------------------------------*/
		if (!p.get_buff()) //если бафа у нас при себе нету
		{
			p.inc_movetimer_buff(time); //начинаем отсчет до выподения бафа
		}
		if (p.get_movetimer_buff() > 77500 && !p.get_buff_drop()) //если прошло 77500 мкс (~ 60 с) и баф НЕ на карте, то создаем баф
		{
			objlist.push_back(new object(buffimage, p.generatorhavki(), 64, 58, 58, 0, 0.25, "buff_1")); //создаем рандомно по X новый объект (баф)
			p.set_buff_drop(true); //говорим, что баф выпал на карту
		}
		/*------------------------------------------------------------------------*/



		  /*--------------------------------------------------------------------------------------------*/
		 /*--------------------Проверка на пересечение с объектами (список objlist)--------------------*/
		/*--------------------------------------------------------------------------------------------*/
		for (it = objlist.begin(); it != objlist.end();) //прогоняем список объектов
		{
			entity* b = *it; //просто, чтобы каждый раз писать не (*it)->, а b->
			b->update(time); //вызываем ф-ию update для всех элементов списка objlist (объектов класса object)
			if (b->get_eaten() == true) { it = objlist.erase(it); delete b; } //проверяем, если соприкоснулись с объектом (eaten == true), то
			else it++; //удаляем из списка и из динамической памяти, в противном случа идем дальше по списку
		}
		for (it = objlist.begin(); it != objlist.end(); it++) //прогоняем список объектов (objlist) от начала до конца
		{
			if ((*it)->getrect().intersects(p.getrect())) //если спрайт объекта соприкасается со спрайтом персонажа
			{
				if ((*it)->get_name() == "eat_1") { p.inc_health(16); p.inc_count_get_eat(); } //если еда, прибавляем 16 хп (считаем еду)
				(*it)->set_eaten(true); //сообщаем, что еда съедена, чтобы в следующий тик она удалилась (или баф подобран)
				if ((*it)->get_name() == "buff_1") //если баф
				{
					p.set_buff(true); //он теперь у нас есть
					p.set_bubbles(); //задаем 2 снаряда
					p.set_movetimer_buff(0); //обнуляем счетчик выпадения бафа
					p.inc_count_get_buff(); //счетчик подобранных бафов
				}
			}
		}
		/*--------------------------------------------------------------------------------------------*/



		  /*----------------------------------------------------------------------------------------------*/
		 /*--------------------Реализация движения снаряда пузыря (выбор направления)--------------------*/
		/*----------------------------------------------------------------------------------------------*/
		for (it = ammolist.begin(); it != ammolist.end();) //проходимся по списку снарядов
		{
			entity* b = *it; //для удобства
			if (b->get_name() == "ammo_right") //если игрок двигался вправо (та самая проверка имени при создании объекта класса cbubble)
			{
				b->update(time); //пускаем пузырь вправо (скорость по X положительная)
			}
			else //иначе, если имя снаряда ammo_left (то есть игрок двигался влево)
			{
				b->update(-time); //пускаем пузырь влево (скорость по X отрицательная)
			}
			if (b->get_bubble_crash()) { it = ammolist.erase(it); delete b; b->set_bubble_crash(false); }
			//если пузырь врезался в стену, удаляем его из списка и из памяти, а так же проверку на столкновения делаем false
			else it++; //иначе шагаем дальше по списку снарядов
		}
		/*----------------------------------------------------------------------------------------------*/



		  /*-----------------------------------------------------------------------------------------------------*/
		 /*--------------------Проверка списка objlist на то, сколько и каких объектов в нем--------------------*/
		/*-----------------------------------------------------------------------------------------------------*/
		if (objlist.size() < 3) //если кусочков еды в списке меньше 3-х
		{
			objlist.push_back(new object(eatimage, p.generatorhavki(), 64, 20, 22, 0, 0.25, "eat_1")); //создаем еду и заносим в конец списка
		}
		else //иначе (если в списке >= 3-х объектов)
		{
			if (objlist.size() == 3) //если объектов в списке ровно 3
			{
				for (it = objlist.begin(); it != objlist.end(); it++) //прогоняем список объектов
				{
					if ((*it)->get_name() != "eat_1") //проверяем: если хотя бы один объект не имеет имя eat_1 (значит на экране 2 еды и баф)
					{
						objlist.push_back(new object(eatimage, p.generatorhavki(), 64, 20, 22, 0, 0.25, "eat_1")); //создаем еду и заносим в конец списка
					}
				}
			}
		}
		/*-----------------------------------------------------------------------------------------------------*/



		  /*---------------------------------------------------------------------------------------*/
		 /*--------------------Взаимодействие акул со снарядами и с персонажем--------------------*/
		/*---------------------------------------------------------------------------------------*/
		for (it = entities.begin(); it != entities.end(); ++it) //прогоняем список врагов (entities) от начала до конца
		{
			for (it_2 = ammolist.begin(); it_2 != ammolist.end(); ++it_2) //прогоняем список снарядов (ammolist)
			{
				entity* b = *it_2; //просто для удобства
				if ((*it)->getrect().intersects(b->getrect())) //если спрайт акулы соприкасается со спрайтом снаряда
				{
					(*it)->set_stun(true); //акула оглушена
					b->set_bubble_crash(true); //сообщаем, что врезались, чтобы пузырь удалился из списка и памяти
					p.inc_count_stun_shark(); //считаем количество оглушений акул
				}
			}
			for (it_2 = ammolist.begin(); it_2 != ammolist.end();) //прогоняем список снарядов (ammolist)
			{
				entity* q = *it_2;
				if (q->get_bubble_crash() == true) { it_2 = ammolist.erase(it_2); delete q; q->set_bubble_crash(false); }
				//если пузырь врезался, удаляем его из списка и памяти, а так же меняем проверку столкновений (bubble_crash = falses)
				else it_2++; //иначе идем дальше по списку
			}
			(*it)->inc_movetimer_stun(time); //начинаем отсчет огушения
			if ((*it)->get_stun()) //если акула оглушена
			{
				if ((*it)->get_movetimer_stun() < 3750) //если 3 секунды оглушения еще НЕ прошло
				{
					(*it)->update(0); //акула обездвижена. Ее скорость в методе update умножается не на время sfml, а на 0, а значит = 0
					if ((*it)->get_check_shark_stun() == 1) //если 1, значит акула плыла влево
					{
						(*it)->set_sprite(126, 48, -126, -48); //переворачиваем спрайт акулы к верху брюхом :)
					}
					else //иначе, если 2 (может быть только 1 или 2), значит акула плыла вправо
					{
						(*it)->set_sprite(0, 48, 126, -48); //переворачиваем спрайт акулы к верху брюхом :)
					}
				}
				else //иначе, если 3 секунды уже прошло
				{
					(*it)->set_movetimer_stun(0); //сбрасываем счетчик
					(*it)->set_stun(false); //говорим, что акула теперь в порядке
					if ((*it)->get_check_shark_stun() == 1) //если 1, значит акула плыла влево
					{
						(*it)->set_sprite(126, 0, -126, 48); //возвращаем акуле нормальное положение спрайта
					}
					else //иначе, если 2 (может быть только 1 или 2), значит акула плыла вправо
					{
						(*it)->set_sprite(0, 0, 126, 48); //возвращаем акуле нормальное положение спрайта
					}
				}
			}
			else //иначе (если акула не оглушена)
			{
				(*it)->update(time); //вызываем метод update для каждого объекта списка entities (для каждого врага), если он не оглушен
				(*it)->set_movetimer_stun(0); //если акула не оглушена, сбрасываем счетчик
			}
			if (!p.get_kusy()) //если мы НЕ находимся под неуязвимостью от кусь
			{
				if ((*it)->getrect().intersects(p.getrect()) && (*it)->get_stun() == false && p.get_life()) //если спрайт врага пересекается с персонажем
				{																							//и акула НЕ оглушена и персонаж жив
					p.dec_health(25); // -25 ХП
					p.set_kusy(true); //КУСЬ!
				}
			}
		}
		/*---------------------------------------------------------------------------------------*/



		if (m.get_check_restart()) //если нажали клавишу "новая игра" во время паузы
		{
			return true; //завершаем ф-ию, возвращая true для создания рекурсии и повторного вызова этой ф-ии is_game (получается новая игра)
		}

		p.update(time); //метод update класса player
		hp.update(p.get_health(), p.get_sprite().getPosition().x, p.get_sprite().getPosition().y); //метод update для шкалы здоровья

		window.setView(view); //устанавливаем камеру в нужное место (сдвигаем на 64 пикселя вниз и вправо, чтобы не было видно границ карты)

		window.clear(); //отчищаем окно от предыдущего кадра

		p.draw_map(window, s_map);

		ostringstream playerhealth, minutesstring, secondsstring, counteat, countbuff, countstun;
		//создаем специальные строки для вывода их на экран (обычные выводятся в консоль) и помещаем в них обычные строки
		playerhealth << p.get_health(); //строка числового вывода здоровья (только для тестов)
		minutesstring << minutes; //строка подсчета минут игры
		secondsstring << seconds; //строка подсчета секунд игры (до 60 секунд)
		counteat << p.get_count_get_eat(); //строка подсчета съеденной еды
		countbuff << p.get_count_get_buff(); //строка подсчета подобранных бафов
		countstun << p.get_count_stun_shark(); //строка подсчета оглушений акул
		if (p.get_life()) //если персонаж жив
		{
			text.setString(/*L"Здоровье: " + playerhealth.str() + */L"Game time: " + minutesstring.str() + L" minutes " + secondsstring.str() + L" seconds "); //выводим на экран
			text.setPosition(1550, 64); //задаем позицию текста на экране
			window.draw(text); //рисуем текст
		}

		for (it = objlist.begin(); it != objlist.end(); ++it) { window.draw((*it)->get_sprite()); } //рисуем еду
		for (it = entities.begin(); it != entities.end(); ++it) { window.draw((*it)->get_sprite()); } //рисуем врагов
		for (it = ammolist.begin(); it != ammolist.end(); ++it) { window.draw((*it)->get_sprite()); } //рисуем пузыри


		window.draw(hp.get_sprite_hp()); //Рисуем полоску здоровья над персонажем
		window.draw(hp.get_black_bar()); //Рисуем черную полоску поверх здоровья

		if (p.get_bubbles() == 2) //если у нас 2 заряда выстрелов
		{
			ammo_count_sprite.setPosition(p.get_sprite().getPosition().x + 12, p.get_sprite().getPosition().y + 46); //позиционируем снаряд
			window.draw(ammo_count_sprite); //рисуем первый снаряд
			ammo_count_sprite.setPosition(p.get_sprite().getPosition().x + 32, p.get_sprite().getPosition().y + 46); //позиционируем снаряд
			window.draw(ammo_count_sprite); //рисуем второй снаряд
		}
		if (p.get_bubbles() == 1) //если у нас 1 заряд выстрела
		{
			ammo_count_sprite.setPosition(p.get_sprite().getPosition().x + 12, p.get_sprite().getPosition().y + 46); //позиционируем снаряд
			window.draw(ammo_count_sprite); //рисуем первый снаряд
		}
		window.draw(p.get_sprite()); //рисуем персонажа

		if (num_pre_start == 3) //проверка на то, какое число выводить на экран до начала игры (3)
			window.draw(ps3_sprite); //собственно рисуем это число (3)
		if (num_pre_start == 2) //проверка на то, какое число выводить на экран до начала игры (2)
			window.draw(ps2_sprite); //собственно рисуем это число (2)
		if (num_pre_start == 1) //проверка на то, какое число выводить на экран до начала игры (1)
			window.draw(ps1_sprite); //собственно рисуем это число (1)
		if (num_pre_start == 4) //проверка на то, какое число выводить на экран до начала игры (надпись)
		{
			//Далее много условий на временнЫе интервалы для создания эффекта постепенного исчезновения надписи (увеличение прозрачности)
			if (count_pre_start > 3750 && count_pre_start <= 4000) //если переменная count_pre_start накопила > 3750 мкс, но <= 4000 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 255)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 4000 && count_pre_start <= 4250) //если переменная count_pre_start накопила > 4000 мкс, но <= 4250 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 220)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 4250 && count_pre_start <= 4500) //если переменная count_pre_start накопила > 4250 мкс, но <= 4500 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 190)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 4500 && count_pre_start <= 4750) //если переменная count_pre_start накопила > 4500 мкс, но <= 4750 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 160)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 4750 && count_pre_start <= 5000) //если переменная count_pre_start накопила > 4750 мкс, но <= 5000 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 130)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 5000 && count_pre_start <= 5250) //если переменная count_pre_start накопила > 5000 мкс, но <= 5250 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 100)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 5250 && count_pre_start <= 5500) //если переменная count_pre_start накопила > 5250 мкс, но <= 5500 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 70)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 5500 && count_pre_start <= 5750) //если переменная count_pre_start накопила > 5500 мкс, но <= 5750 мкс
			{
				ps0_sprite.setColor(Color(255, 255, 255, 40)); //задаем цвет и прозрачность спрайту с надписью (R, G, B, прозр-ть)
				window.draw(ps0_sprite); //рисуем эту надпись
			}
			if (count_pre_start > 5750) //если переменная count_pre_start накопила > 5750 мкс
			{
				num_pre_start = 0; //сообщаем, что num_pre_start, ранее отвечающая за то, какую картинку выводить до начала игры = 0
			}
		}
		if (num_pre_start == 0) //если никакую картинку уже рисовать не нужно
			count_pre_start = 0; //обнуляем счетчик времени, отвечающий за интервал смены картинок до начала игры, чтобы он не рос во время игры

		if (p.get_pause() || !p.get_life()) //если стоит пауза или если НЕ живи
		{
			//Сюда вставить метод класса menu, меняющий переменную на true, за счет чего будут выводиться кнопки меню из другого метода
			m.pause_func(window); //вызываем ф-ию для реализации кнопок меню паузы (при смерти те же самые кнопки)
			window.draw(m.get_pause_sprite_1()); //рисуем первую кнопку
			window.draw(m.get_pause_sprite_2()); //рисуем вторую кнопку
		}

		if (!p.get_life()) //если игрок умер выводим статистику
		{
			text.setString(L"Your result: " + minutesstring.str() + L" minutes " + secondsstring.str() + L" seconds " + L"\nFood taken: " + counteat.str() + L"\nBuff taken: " + countbuff.str() + L"\nStunned sharks: " + countstun.str()); //выводим на экран
			text.setPosition(598, 440); //задаем позицию текста на экране
			window.draw(text); //рисуем текст
		}

		window.display(); //отображаем все что можно и что нельзя
	}
	return false;
}

void restart_func(RenderWindow& window) //промежуточная ф-ия для создания рекурсии (для возможности перезапуска игры)
{
	if (is_game(window)) //если основная ф-ия is_game возвращает true (в случае нажатия кнопки новая игра)
		restart_func(window); //вызываем ф-ию restart_func, которая в свую очередь снова вызовет is_game
}