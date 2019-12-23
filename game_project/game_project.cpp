#include <iostream> //что-то
#include <sstream> //еще что-то
#include <list> //подключаем списки библиотеки STL
#include <SFML/Graphics.hpp> //подключаем графику sfml
#include <ctime> //подключаем время
#include <math.h> //библиотека с математическими функциями
#include "map.h" //подключаем файл с картой
using namespace std; //пространство имен std, чтобы не писать каждый раз std::
using namespace sf; //пространство имен sf, чтобы не писать каждый раз sf::

class entity //класс родитель
{
protected:
	float x, y, dx, dy; //позиция по (x, y); скорость по (dx, dy);
	float sweem, speed, movetimer; //эффект плавания; скорость; счетчик для разного
	int health, w, h; //здоровье; ширина спрайта; высота спрайта
	int count_gametime; //счетчик времени игры
	bool life; //игрок жив или нет
	Texture texture; //текстура
	Sprite sprite; //спрайт
	String name; //имя объекта
public:
	entity(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) //конструктор с параметрами
	{
		x = X; y = Y; w = W; h = H; name = Name; dx = Dx; dy = Dy;
		movetimer = 0; health = 100; sweem = 0; count_gametime = 0;
		life = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
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
				speed = 0.15; //скорость движения
				dx = -speed; dy = 0; //скорость по координатам
				control(time, dx, dy); //вызываем ф-ию control и передаем ей время и скорость по координатам
			}
			if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) //движение вправо (кнопка D)
			{
				sprite.setTextureRect(IntRect(0, 0, 60, 42)); //повернуть спрайт рыбы вправо
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
	void inc_count_gametime()
	{
		count_gametime++;
	}
	int get_count_gametime()
	{
		return count_gametime;
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
					}
					if (Dx < 0)
					{
						x = j * 64 + 64; dx = -Dx; //если акула врезалась в препятствие, то разворачиваем ее (вправо)
						sprite.setTextureRect(IntRect(0, 0, 126, 48));//разворачиваем спрайт вправо
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

		sweem += time; //переменная и условия для эффекта плавания
		if (sweem > 100) sprite.setPosition(x, y + 3); //+150
		if (sweem > 250) sprite.setPosition(x, y + 3); //+300
		if (sweem > 550) sprite.setPosition(x, y - 3); //+450
		if (sweem > 1000) { sprite.setPosition(x, y - 3); sweem = 0; }
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

	list<entity*> entities; //динамический список для помещения туда врагов
	list<entity*>::iterator it; //итератор для этого списка

	entities.push_back(new enemy(enemyoneimage, 64, 128, 126, 48, 0.2, 0, "enemy_1")); //враг 1

	player p(heroimage, 64, 64, 60, 42, 0.15, 0, "player_1"); //создаем персонажа (объект подкласса player)

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
		}

		for (it = entities.begin(); it != entities.end(); ++it) //прогоняем список врагов (entities) от начала до конца
		{
			(*it)->update(time); //вызываем метод update для каждого объекта списка entities (для каждого врага), если он не оглушен
		}

		p.update(time); //метод update класса player

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

		for (it = entities.begin(); it != entities.end(); ++it) { window.draw((*it)->get_sprite()); } //рисуем врагов
		window.draw(p.get_sprite()); //рисуем персонажа

		window.display(); //отображаем все что можно и что нельзя
	}
	return false;
}

void restart_func(RenderWindow & window) //промежуточная ф-ия для создания рекурсии (для возможности перезапуска игры)
{
	if (is_game(window)) //если основная ф-ия is_game возвращает true (в случае нажатия кнопки новая игра)
		restart_func(window); //вызываем ф-ию restart_func, которая в свую очередь снова вызовет is_game
}

int main() //основная ф-ия (всегда вызывается первой в программе)
{
	RenderWindow window(VideoMode(/*640*/1920, /*480*/1080), "GlassHouse", Style::Fullscreen); //создаем окно (разрешение, название, на полный экран)
	restart_func(window); //все остальное
	return 0; //вот и сказочке конец! А кто работал над ней, пойдет спать... Короче возвращаем 0, если все хорошо. (хоть где-то 0 - это хорошо)
}