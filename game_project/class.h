#pragma once

using namespace std; //пространство имен std, чтобы не писать каждый раз std::
using namespace sf; //пространство имен sf, чтобы не писать каждый раз sf::

class entity //класс родитель
{
protected:
	float x, y, dx, dy; //позиция по (x, y); скорость по (dx, dy);
	float sweem, speed, movetimer; //эффект плавания; скорость; счетчик для разного
	float movetimer_buff; //отсчет до выпадения бафа
	float movetimer_stun; //отсчет до конца стана
	float movetimer_health; //создаана, чтобы накапливать в себе секунду, после чего отнимается здоровье
	int health, w, h; //здоровье; ширина спрайта; высота спрайта
	int randomx; //случайный элемент по x
	int bubbles; //счетчик зарядов пузырей (максимум 2)
	int count_gametime; //счетчик времени игры
	int count_get_eat, count_get_buff, count_stun_shark; //счетчик собранной еды, бафов и оглушений акул (для статистики)
	bool life; //игрок жив или нет
	bool eaten; //еда съедена или нет
	bool kusy; //акула кусь или не кусь
	bool buff; //баф есть у нас или нету
	bool buff_drop; //баф выпал на карту или нет
	bool bubble_crash; //пузырь столкнулся со стеной или нет
	bool stun; //акула оглушена или нет
	bool pause; //пауза или нет
	bool pre_start; //создана для 3-х секундного отсчета времени перед началом игры (true отсчет идет / false игра началась)
	int check_bubble_fly; //проверка на то, в какую сторону лететь пузырю (зависит от того, куда плыл персонаж)
	int check_shark_stun; //проверка на то, в какую сторону должна смотреть оглушенная акула (зависит от того, куда она плыла)
	Texture texture; //текстура
	Sprite sprite; //спрайт
	String name; //имя объекта
public:
	entity(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name); //конструктор с параметрами

	FloatRect getrect(); //ф-ия получения прямоугольной области спрайта (для проверки столкновений)
	void set_sprite(int q, int w, int e, int r); //задать спрайт кусочно (часть от целого изображения, ну или все изображение)
	bool get_eaten(); //пересеклись ли мы с объектом класса object (с едой или бафом)
	void set_eaten(bool arg); //задать значение пересеклись или нет с объектом класса object
	bool get_bubble_crash(); //узнать, врезался ли летящий снаряд (пузырь) в препятствие
	void set_bubble_crash(bool BC); //задать значение врезался ли летящий снаряд в препятствие
	bool get_stun(); //получить значение: акула оглушена или нет
	void set_stun(bool Stun); //задать значение: акула оглушена или нет
	float get_movetimer_stun(); //получить значение: длительность оглушения акулы
	void set_movetimer_stun(float Stun); //задать значение: длительность стана акулы
	void inc_movetimer_stun(float time); //увеличить значение: длительность стана акулы
	int get_check_shark_stun(); //получить значение: в какую сторону оглушена акула

	virtual void update(float time) = 0; //виртуальная ф-ия update для переопределения в дочерних классах
	virtual Sprite get_sprite() = 0; //виртуальная ф-ия get_sprite для переопределения в дочерних классах
	virtual String get_name() = 0; //виртуальная ф-ия get_name для переопределения в дочерних классах
};

class player :public entity //дочерний класс (игрок)
{
public:
	player(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// :entity(image, X, Y, W, H, Dx, Dy, Name) {}; //конструктор

	void control(float time, float Dx, float Dy); //перемещение персонажа
	void physical_obj(float Dx, float Dy); //проверка на столкновения с объектом
	int generatorhavki();
	void update(float time);
	Sprite get_sprite();
	String get_name();
	bool get_life();
	void dec_health(int znach);
	void inc_health(int znach);
	int get_health();
	void set_kusy(bool Kusy);
	bool get_kusy();
	float get_movetimer_buff();
	void set_movetimer_buff(float Movetimerbuff);
	void inc_movetimer_buff(float time);
	bool get_buff();
	void set_buff(bool Buff);
	void set_bubbles();
	void dec_bubbles();
	int get_bubbles();
	int get_check_bubble_fly();
	bool get_buff_drop();
	void set_buff_drop(bool BD);
	void set_pause();
	bool get_pause();
	int get_count_gametime();
	void inc_count_gametime();
	bool get_pre_start();
	void set_pre_start();
	void inc_movetimer_health(float znach);
	void zero_movetimer_health();
	float get_movetimer_health();
	int get_count_get_eat();
	int get_count_get_buff();
	int get_count_stun_shark();
	void inc_count_get_eat();
	void inc_count_get_buff();
	void inc_count_stun_shark();
	void draw_map(RenderWindow& window, Sprite s_map);
};

class enemy :public entity //дочерний класс (враг)
{
public:
	enemy(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// : entity(image, X, Y, W, H, Dx, Dy, Name); //конструктор

	void physical_obj(float Dx, float Dy); //проверка на столкновения с объектом
	void update(float time);
	Sprite get_sprite();
	String get_name();

};

class object :public entity //дочерний класс (объекты: еда, пузырь)
{
public:
	object(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// :entity(image, X, Y, W, H, Dx, Dy, Name) {  };

	void physical_obj(float Dx, float Dy); //проверка на столкновения с объектом
	void update(float time);
	Sprite get_sprite();
	String get_name();
};

class cbubble :public entity
{
public:
	cbubble(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// :entity(image, X, Y, W, H, Dx, Dy, Name) {};

	void physical_obj(float Dx, float Dy); //проверка на столкновения с объектом
	void update(float time);
	Sprite get_sprite();
	String get_name();
};

void restart_func(RenderWindow& window);
bool is_game(RenderWindow& window); //ф-ия is_game, принимающая в качестве параметра ссылку на объект окно из ф-ии main. Там же она и вызывается