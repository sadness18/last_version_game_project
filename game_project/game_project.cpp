#include <iostream> //что-то
#include <sstream> //еще что-то
#include <list> //подключаем списки библиотеки STL
#include <SFML/Graphics.hpp> //подключаем графику sfml
#include <ctime> //подключаем время
#include <math.h> //библиотека с математическими функциями
#include "map.h" //подключаем файл с картой
#include "life_bar.h"
#include "menu.h"
using namespace std; //пространство имен std, чтобы не писать каждый раз std::
using namespace sf; //пространство имен sf, чтобы не писать каждый раз sf::

class entity //класс родитель
{
protected:
	float x, y, dx, dy; //позиция по (x, y); скорость по (dx, dy);
	float sweem, speed, movetimer; //эффект плавания; скорость; счетчик для разного
	float movetimer_buff; //отсчет до выпадения бафа
	float movetimer_stun; //отсчет до конца стана
	int health, w, h; //здоровье; ширина спрайта; высота спрайта
	int count_gametime; //счетчик времени игры
	int randomx; //случайный элемент по x
	int bubbles; //счетчик зарядов пузырей (максимум 2)
	int check_bubble_fly; //проверка на то, в какую сторону лететь пузырю (зависит от того, куда плыл персонаж)
	int check_shark_stun; //проверка на то, в какую сторону должна смотреть оглушенная акула (зависит от того, куда она плыла)
	bool life; //игрок жив или нет
	bool eaten; //еда съедена или нет
	bool kusy; //акула кусь или не кусь
	bool buff; //баф есть у нас или нету
	bool buff_drop; //баф выпал на карту или нет
	bool bubble_crash; //пузырь столкнулся со стеной или нет
	bool stun; //акула оглушена или нет
	Texture texture; //текстура
	Sprite sprite; //спрайт
	String name; //имя объекта
public:
	entity(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) //конструктор с параметрами
	{
		x = X; y = Y; w = W; h = H; name = Name; dx = Dx; dy = Dy;
		movetimer = 0; health = 50; sweem = 0; count_gametime = 0; randomx = 0; movetimer_buff = 0; bubbles = 0; check_bubble_fly = 0;
		check_shark_stun = 0; movetimer_stun = 0;
		life = true; eaten = false; kusy = false; buff = false; buff_drop = false; bubble_crash = false; stun = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}

	FloatRect getrect() //ф-ия получения прямоугольной области спрайта (для проверки столкновений)
	{
		return FloatRect(x, y, w, h);
	}
	void set_sprite(int q, int w, int e, int r) //задать спрайт кусочно (часть от целого изображения, ну или все изображение)
	{
		sprite.setTextureRect(IntRect(q, w, e, r));
	}
	bool get_eaten() //пересеклись ли мы с объектом класса object (с едой или бафом)
	{
		return eaten;
	}
	void set_eaten(bool arg) //задать значение пересеклись или нет с объектом класса object
	{
		eaten = arg;
	}
	bool get_bubble_crash() //узнать, врезался ли летящий снаряд (пузырь) в препятствие
	{
		return bubble_crash;
	}
	void set_bubble_crash(bool BC) //задать значение врезался ли летящий снаряд в препятствие
	{
		bubble_crash = BC;
	}
	bool get_stun() //получить значение: акула оглушена или нет
	{
		return stun;
	}
	void set_stun(bool Stun) //задать значение: акула оглушена или нет
	{
		stun = Stun;
	}
	float get_movetimer_stun() //получить значение: длительность оглушения акулы
	{
		return movetimer_stun;
	}
	void set_movetimer_stun(float Stun) //задать значение: длительность стана акулы
	{
		movetimer_stun = Stun;
	}
	void inc_movetimer_stun(float time) //увеличить значение: длительность стана акулы
	{
		movetimer_stun += time;
	}
	int get_check_shark_stun() //получить значение: в какую сторону оглушена акула
	{
		return check_shark_stun;
	}

	virtual void update(float time) = 0; //виртуальная ф-ия update для переопределения в дочерних классах
	virtual Sprite get_sprite() = 0; //виртуальная ф-ия get_sprite для переопределения в дочерних классах
	virtual String get_name() = 0; //виртуальная ф-ия get_name для переопределения в дочерних классах
};

class player :public entity //дочерний класс (игрок)
{
public:
	player(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name) //конструктор
	{
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
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

	void control(float time, float Dx, float Dy) //перемещение персонажа
	{
		x += Dx * time;
		y += Dy * time;
		speed = 0;
		sprite.setPosition(x, y);
		physical_obj(Dx, Dy);
	}

	void update(float time)
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

	int generatorhavki()
	{
		randomx = 84 + rand() % 1836; //генератор еды. Числа такие, потому что нужно исключить вероятность краев
		cout << randomx << endl;	 // и -20 пикселей слева и справа от краев видимой области (проще говоря FHD экран 1920 по иксу
		return randomx;				//  а хавка генерируется в диапазоне с 84-го по 1836 пиксель включительно)
	}

	Sprite get_sprite()
	{
		return sprite;
	}
	String get_name()
	{
		return name;
	}
	bool get_life()
	{
		return life;
	}
	int get_health()
	{
		return health;
	}
	void dec_health(int znach)
	{
		health -= znach;
	}
	void inc_count_gametime()
	{
		count_gametime++;
	}
	int get_count_gametime()
	{
		return count_gametime;
	}
	void inc_health(int znach)
	{
		health += znach;
	}
	void set_kusy(bool Kusy)
	{
		kusy = Kusy;
	}
	bool get_kusy()
	{
		return kusy;
	}
	void inc_movetimer_buff(float time)
	{
		movetimer_buff += time;
	}
	bool get_buff()
	{
		return buff;
	}
	void set_buff_drop(bool BD)
	{
		buff_drop = BD;
	}
	bool get_buff_drop()
	{
		return buff_drop;
	}
	float get_movetimer_buff()
	{
		return movetimer_buff;
	}
	void set_buff(bool Buff)
	{
		buff = Buff;
	}
	void set_bubbles()
	{
		bubbles = 2;
	}
	void dec_bubbles()
	{
		bubbles -= 1;
	}
	int get_bubbles()
	{
		return bubbles;
	}
	void set_movetimer_buff(float Movetimerbuff)
	{
		movetimer_buff = Movetimerbuff;
	}
	int get_check_bubble_fly()
	{
		return check_bubble_fly;
	}
};

class enemy :public entity //дочерний класс (враг)
{
public:
	enemy(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name) //конструктор
	{
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
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

	void update(float time)
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

	Sprite get_sprite()
	{
		return sprite;
	}
	String get_name()
	{
		return name;
	}
};

class object :public entity //дочерний класс (объекты: еда, пузырь)
{
public:
	object(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name)
	{
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
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

	void update(float time)
	{
		y += dy * time;
		physical_obj(0, dy);
		sprite.setPosition(x, y);
	}

	Sprite get_sprite()
	{
		return sprite;
	}
	String get_name()
	{
		return name;
	}
};

class cbubble :public entity
{
public:
	cbubble(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) :entity(image, X, Y, W, H, Dx, Dy, Name)
	{
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void physical_obj(float Dx, float Dy) //проверка на столкновения с объектом
	{
		for (int i = y / 64; i < (y + h) / 64; i++)
		{
			for (int j = x / 64; j < (x + w) / 64; j++)
			{
				if (TileMap[i][j] == '0' || TileMap[i][j] == 'd')
				{
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

	void update(float time)
	{
		x += dx * time;
		physical_obj(dx, 0);
		sprite.setPosition(x, y);
	}

	Sprite get_sprite()
	{
		return sprite;
	}
	String get_name()
	{
		return name;
	}
};

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

bool is_game(RenderWindow& window) //ф-ия is_game, принимающая в качестве параметра ссылку на объект окно из ф-ии main. Там же она и вызывается
{
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

	Clock clock; //переменная времени
	int minutes = 0; //кол-во минут, прошедших с момента начала игры
	int seconds = 0; //кол-во секунд, которые переводятся в минуты, когда достигают 60
	int save_time = 0; //для сохранения времени

	  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 ////////////////////////////////////////////////////ПОКА ОТКРЫТО ОКНО////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen())
	{
		float time; //переменная для занесения в нее времени, для последующей реализации передвижения спрайтов (т.н. привязка ко времени)
		time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время (обнуляет)
		time = time / 800; //скорость игры. Для справки (напоминалка) --> 1 000 000 мкс (1 с) / 800 = 1250 мкс (1 с)

		if (p.get_life()) //если игрок жив
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

		Event event; //объект класса event
		while (window.pollEvent(event)) //пока что-то там event
		{
			if (event.type == Event::Closed) //если event закрылся
				window.close(); //закрываем окно

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
		}

		if (!p.get_buff()) //если бафа у нас при себе нету
		{
			p.inc_movetimer_buff(time); //начинаем отсчет до выподения бафа
		}
		if (p.get_movetimer_buff() > 12500 && !p.get_buff_drop()) //если прошло 77500 мкс (~ 60 с) и баф НЕ на карте, то создаем баф
		{
			objlist.push_back(new object(buffimage, p.generatorhavki(), 64, 58, 58, 0, 0.25, "buff_1")); //создаем рандомно по X новый объект (баф)
			p.set_buff_drop(true); //говорим, что баф выпал на карту
		}

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
				if ((*it)->get_name() == "eat_1") { p.inc_health(16); } //если еда, прибавляем 16 хп (считаем еду)
				(*it)->set_eaten(true); //сообщаем, что еда съедена, чтобы в следующий тик она удалилась (или баф подобран)
				if ((*it)->get_name() == "buff_1") //если баф
				{
					p.set_buff(true); //он теперь у нас есть
					p.set_bubbles(); //задаем 2 снаряда
					p.set_movetimer_buff(0); //обнуляем счетчик выпадения бафа
				}
			}
		}

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

		for (it = entities.begin(); it != entities.end(); ++it) //прогоняем список врагов (entities) от начала до конца
		{
			for (it_2 = ammolist.begin(); it_2 != ammolist.end(); ++it_2) //прогоняем список снарядов (ammolist)
			{
				entity* b = *it_2; //просто для удобства
				if ((*it)->getrect().intersects(b->getrect())) //если спрайт акулы соприкасается со спрайтом снаряда
				{
					(*it)->set_stun(true); //акула оглушена
					b->set_bubble_crash(true); //сообщаем, что врезались, чтобы пузырь удалился из списка и памяти
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
					p.dec_health(10); // -10 ХП
					p.set_kusy(true); //КУСЬ!
				}
			}
		}

		p.update(time); //метод update класса player
		hp.update(p.get_health(), p.get_sprite().getPosition().x, p.get_sprite().getPosition().y); //метод update для шкалы здоровья

		window.setView(view); //устанавливаем камеру в нужное место (сдвигаем на 64 пикселя вниз и вправо, чтобы не было видно границ карты)

		window.clear(); //отчищаем окно от предыдущего кадра

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

		ostringstream playerhealth, minutesstring, secondsstring, counteat, countbuff, countstun;
		//создаем специальные строки для вывода их на экран (обычные выводятся в консоль) и помещаем в них обычные строки
		playerhealth << p.get_health(); //строка числового вывода здоровья (только для тестов)
		minutesstring << minutes; //строка подсчета минут игры
		secondsstring << seconds; //строка подсчета секунд игры (до 60 секунд)
		if (p.get_life()) //если персонаж жив
		{
			text.setString(L"Здоровье: " + playerhealth.str() + L"\nВремя игры: " + minutesstring.str() + L" минут(ы) " + secondsstring.str() + L" секунд(ы) "); //выводим на экран
			text.setPosition(1500, 64); //задаем позицию текста на экране
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

		window.display(); //отображаем все что можно и что нельзя
	}
	return false;
}

void restart_func(RenderWindow& window) //промежуточная ф-ия для создания рекурсии (для возможности перезапуска игры)
{
	if (is_game(window)) //если основная ф-ия is_game возвращает true (в случае нажатия кнопки новая игра)
		restart_func(window); //вызываем ф-ию restart_func, которая в свую очередь снова вызовет is_game
}

int main() //основная ф-ия (всегда вызывается первой в программе)
{
	RenderWindow window(VideoMode(/*640*/1920, /*480*/1080), "GlassHouse", Style::Fullscreen); //создаем окно (разрешение, название, на полный экран)
	menu m; //объект класса меню
	m.menu_func(window); //ф-ия вызова меню
	restart_func(window); //все остальное
	return 0; //вот и сказочке конец! А кто работал над ней, пойдет спать... Короче возвращаем 0, если все хорошо. (хоть где-то 0 - это хорошо)
}