#pragma once

using namespace std; //������������ ���� std, ����� �� ������ ������ ��� std::
using namespace sf; //������������ ���� sf, ����� �� ������ ������ ��� sf::

class entity //����� ��������
{
protected:
	float x, y, dx, dy; //������� �� (x, y); �������� �� (dx, dy);
	float sweem, speed, movetimer; //������ ��������; ��������; ������� ��� �������
	float movetimer_buff; //������ �� ��������� ����
	float movetimer_stun; //������ �� ����� �����
	float movetimer_health; //��������, ����� ����������� � ���� �������, ����� ���� ���������� ��������
	int health, w, h; //��������; ������ �������; ������ �������
	int randomx; //��������� ������� �� x
	int bubbles; //������� ������� ������� (�������� 2)
	int count_gametime; //������� ������� ����
	int count_get_eat, count_get_buff, count_stun_shark; //������� ��������� ���, ����� � ��������� ���� (��� ����������)
	bool life; //����� ��� ��� ���
	bool eaten; //��� ������� ��� ���
	bool kusy; //����� ���� ��� �� ����
	bool buff; //��� ���� � ��� ��� ����
	bool buff_drop; //��� ����� �� ����� ��� ���
	bool bubble_crash; //������ ���������� �� ������ ��� ���
	bool stun; //����� �������� ��� ���
	bool pause; //����� ��� ���
	bool pre_start; //������� ��� 3-� ���������� ������� ������� ����� ������� ���� (true ������ ���� / false ���� ��������)
	int check_bubble_fly; //�������� �� ��, � ����� ������� ������ ������ (������� �� ����, ���� ���� ��������)
	int check_shark_stun; //�������� �� ��, � ����� ������� ������ �������� ���������� ����� (������� �� ����, ���� ��� �����)
	Texture texture; //��������
	Sprite sprite; //������
	String name; //��� �������
public:
	entity(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name); //����������� � �����������

	FloatRect getrect(); //�-�� ��������� ������������� ������� ������� (��� �������� ������������)
	void set_sprite(int q, int w, int e, int r); //������ ������ ������� (����� �� ������ �����������, �� ��� ��� �����������)
	bool get_eaten(); //����������� �� �� � �������� ������ object (� ���� ��� �����)
	void set_eaten(bool arg); //������ �������� ����������� ��� ��� � �������� ������ object
	bool get_bubble_crash(); //������, �������� �� ������� ������ (������) � �����������
	void set_bubble_crash(bool BC); //������ �������� �������� �� ������� ������ � �����������
	bool get_stun(); //�������� ��������: ����� �������� ��� ���
	void set_stun(bool Stun); //������ ��������: ����� �������� ��� ���
	float get_movetimer_stun(); //�������� ��������: ������������ ��������� �����
	void set_movetimer_stun(float Stun); //������ ��������: ������������ ����� �����
	void inc_movetimer_stun(float time); //��������� ��������: ������������ ����� �����
	int get_check_shark_stun(); //�������� ��������: � ����� ������� �������� �����

	virtual void update(float time) = 0; //����������� �-�� update ��� ��������������� � �������� �������
	virtual Sprite get_sprite() = 0; //����������� �-�� get_sprite ��� ��������������� � �������� �������
	virtual String get_name() = 0; //����������� �-�� get_name ��� ��������������� � �������� �������
};

class player :public entity //�������� ����� (�����)
{
public:
	player(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// :entity(image, X, Y, W, H, Dx, Dy, Name) {}; //�����������

	void control(float time, float Dx, float Dy); //����������� ���������
	void physical_obj(float Dx, float Dy); //�������� �� ������������ � ��������
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

class enemy :public entity //�������� ����� (����)
{
public:
	enemy(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// : entity(image, X, Y, W, H, Dx, Dy, Name); //�����������

	void physical_obj(float Dx, float Dy); //�������� �� ������������ � ��������
	void update(float time);
	Sprite get_sprite();
	String get_name();

};

class object :public entity //�������� ����� (�������: ���, ������)
{
public:
	object(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// :entity(image, X, Y, W, H, Dx, Dy, Name) {  };

	void physical_obj(float Dx, float Dy); //�������� �� ������������ � ��������
	void update(float time);
	Sprite get_sprite();
	String get_name();
};

class cbubble :public entity
{
public:
	cbubble(Image& image, float X, float Y, int W, int H, float Dx, float Dy, String Name);// :entity(image, X, Y, W, H, Dx, Dy, Name) {};

	void physical_obj(float Dx, float Dy); //�������� �� ������������ � ��������
	void update(float time);
	Sprite get_sprite();
	String get_name();
};

void restart_func(RenderWindow& window);
bool is_game(RenderWindow& window); //�-�� is_game, ����������� � �������� ��������� ������ �� ������ ���� �� �-�� main. ��� �� ��� � ����������