#include <SFML/Graphics.hpp>
using namespace sf;

class life_bar //����� ��� ���������� ����� �������� ��� ���������� � �� ��������� � ����������� �� �������� ��������
{
private:
	Image image; //�����������
	Texture texture; //��������
	Sprite sprite; //������
	int max; //������������ �������� ��������
	RectangleShape bar; //������ �������������, ������������� ����� �������� � ����������� �� �������� ��������
public:
	life_bar() //����������� ��� ����������
	{
		image.loadFromFile("images/hp_bar.png"); //��������� �������� ����� ��������
		texture.loadFromImage(image); //�� � ��������
		sprite.setTexture(texture); //� � ������
		sprite.setTextureRect(IntRect(0, 0, 54, 12)); //��� ����� �� ��� ��������, ��� ��� �������� ������ �� �����

		bar.setFillColor(Color(0, 0, 0)); //������ �������������
		max = 100; //�������� 100 ��. �������� ����� ����
	}

	void update(int k, float pos_x, float pos_y) //�-�� ��������� ��������� �������� �������� (k), � ��� �� ��������� ���������
	{
		if (k >= 0) //���� �������� �� �������������
		{
			if (k <= max) //���� �������� ������, ���� = 100
			{
				bar.setSize(Vector2f((max - k) * -50 / max, 8)); //������ ������ ������� ��������������. �������� 50 px = 0 HP; 0 px = 100 HP
			}
			sprite.setPosition(pos_x + 3, pos_y - 16); //������������� hp bar ��� ����������
			bar.setPosition(pos_x + 55, pos_y - 14); //������������� �����, �� ������� �������� ���������� ������ �������������
		}
	}

	Sprite get_sprite_hp() //�-�� ��������� ������� ����� ��������
	{
		return sprite;
	}

	RectangleShape get_black_bar() //�-�� ��������� ������� ��������������
	{
		return bar;
	}
};
