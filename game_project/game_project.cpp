#include <iostream> //что-то
#include <sstream> //еще что-то
#include <list> //подключаем списки библиотеки STL
#include <SFML/Graphics.hpp> //подключаем графику sfml
#include <ctime> //подключаем время
#include <math.h> //библиотека с математическими функциями
using namespace std; //пространство имен std, чтобы не писать каждый раз std::
using namespace sf; //пространство имен sf, чтобы не писать каждый раз sf::

class entity //класс родитель
{
protected:
	float x, y, dx, dy; //позиция по (x, y); скорость по (dx, dy);
	float sweem, speed, movetimer; //эффект плавания; скорость; счетчик для разного
	int health, w, h; //здоровье; ширина спрайта; высота спрайта
	bool life; //игрок жив или нет
	Texture texture; //текстура
	Sprite sprite; //спрайт
	String name; //имя объекта
public:
	entity(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name) //конструктор с параметрами
	{
		x = X; y = Y; w = W; h = H; name = Name; dx = Dx; dy = Dy;
		movetimer = 0; health = 100; sweem = 0;
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

	void control(float time, float Dx, float Dy) //перемещение персонажа
	{
		x += Dx * time; //изменяем координату х на скорость, умноженную на время
		y += Dy * time; //изменяем координату у на скорость, умноженную на время
		speed = 0; //делаем скорость 0, чтобы персонаж останавливался, когда мы отпускаем клавишу
		sprite.setPosition(x, y); //задаем позицию спрайту
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
	Image heroimage; //объект изображение (для игрока)
	heroimage.loadFromFile("images/fish.png"); //выбираем изображение для игрока

	player p(heroimage, 64, 64, 60, 42, 0.15, 0, "player_1"); //создаем персонажа (объект подкласса player)

	Clock clock; //переменная времени

	  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 ////////////////////////////////////////////////////ПОКА ОТКРЫТО ОКНО////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen())
	{
		float time; //переменная для занесения в нее времени, для последующей реализации передвижения спрайтов (т.н. привязка ко времени)
		time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время (обнуляет)
		time = time / 800; //скорость игры. Для справки (напоминалка) --> 1 000 000 мкс (1 с) / 800 = 1250 мкс (1 с)

		Event event; //объект класса event
		while (window.pollEvent(event)) //пока что-то там event
		{
			if (event.type == Event::Closed) //если event закрылся
				window.close(); //закрываем окно
		}

		p.update(time); //метод update класса player

		window.clear(); //отчищаем окно от предыдущего кадра

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
	RenderWindow window(VideoMode(640, 480), "GlassHouse"/*, Style::Fullscreen*/); //создаем окно (разрешение, название, на полный экран)
	restart_func(window); //все остальное
	return 0; //вот и сказочке конец! А кто работал над ней, пойдет спать... Короче возвращаем 0, если все хорошо. (хоть где-то 0 - это хорошо)
}