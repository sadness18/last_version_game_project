#include <SFML/Graphics.hpp>
using namespace sf;

class life_bar //класс для реализации шкалы здоровья над персонажем и ее изменения в зависимости от значения здоровья
{
private:
	Image image; //изображение
	Texture texture; //текстура
	Sprite sprite; //спрайт
	int max; //максимальное значение здоровья
	RectangleShape bar; //черный прямоугольник, перекрывающий шкалу здоровья в зависимости от значения здоровья
public:
	life_bar() //конструктор без параметров
	{
		image.loadFromFile("images/hp_bar.png"); //загружаем картинку шкалы здоровья
		texture.loadFromImage(image); //ее в текстуру
		sprite.setTexture(texture); //и в спрайт
		sprite.setTextureRect(IntRect(0, 0, 54, 12)); //нам нужна не вся картинка, так что выбираем только ее часть

		bar.setFillColor(Color(0, 0, 0)); //Черный прямоугольник
		max = 100; //максимум 100 ед. здоровья может быть
	}

	void update(int k, float pos_x, float pos_y) //ф-ия принимает параметры текущего здоровья (k), а так же координат персонажа
	{
		if (k >= 0) //если здоровье не отрицательное
		{
			if (k <= max) //если здоровье меньше, либо = 100
			{
				bar.setSize(Vector2f((max - k) * -50 / max, 8)); //задаем размер черного прямоугольника. Максимум 50 px = 0 HP; 0 px = 100 HP
			}
			sprite.setPosition(pos_x + 3, pos_y - 16); //позиционируем hp bar над персонажем
			bar.setPosition(pos_x + 55, pos_y - 14); //позиционируем точку, от которой начинает рисоваться черный прямоугольник
		}
	}

	Sprite get_sprite_hp() //ф-ия получения спрайта шкалы здоровья
	{
		return sprite;
	}

	RectangleShape get_black_bar() //ф-ия получения черного прямоугольника
	{
		return bar;
	}
};
