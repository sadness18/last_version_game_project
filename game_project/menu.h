#include <SFML/Graphics.hpp>
using namespace sf;

class menu
{
private:
	Texture menu_texture_1, menu_texture_2, menu_texture_3, about_texture, menu_back;
	Sprite menu_sprite_1, menu_sprite_2, menu_sprite_3, about_sprite, background_sprite;
	bool is_menu;
	bool check_pause;
	int menu_num;
public:
	menu()
	{
		is_menu = true; menu_num = 0;
		check_pause = false;

		menu_texture_1.loadFromFile("images/m1.png");
		menu_texture_2.loadFromFile("images/m2.png");
		menu_texture_3.loadFromFile("images/m3.png");
		about_texture.loadFromFile("images/ab.png");
		menu_back.loadFromFile("images/bk.png");
		menu_sprite_1.setTexture(menu_texture_1);
		menu_sprite_2.setTexture(menu_texture_2);
		menu_sprite_3.setTexture(menu_texture_3);
		about_sprite.setTexture(about_texture);
		background_sprite.setTexture(menu_back);

		menu_sprite_1.setPosition(752, 360);
		menu_sprite_2.setPosition(752, 471);
		menu_sprite_3.setPosition(752, 582);
		background_sprite.setPosition(0, 0);
	}

	void menu_func(RenderWindow& window)
	{
		is_menu = true; //???
		while (is_menu)
		{
			menu_texture_1.loadFromFile("images/m1.png");
			menu_texture_2.loadFromFile("images/m2.png");
			menu_texture_3.loadFromFile("images/m3.png");

			menu_num = 0;
			window.clear(Color(129, 181, 221));

			if (IntRect(752, 360, 416, 111).contains(Mouse::getPosition(window))) { menu_texture_1.loadFromFile("images/m11.png"); menu_num = 1; }
			if (IntRect(752, 471, 416, 111).contains(Mouse::getPosition(window))) { menu_texture_2.loadFromFile("images/m22.png"); menu_num = 2; }
			if (IntRect(752, 582, 416, 111).contains(Mouse::getPosition(window))) { menu_texture_3.loadFromFile("images/m33.png"); menu_num = 3; }

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (menu_num == 1) is_menu = false;
				if (menu_num == 2) { window.draw(about_sprite); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
				if (menu_num == 3) { window.close(); is_menu = false; } //при нажатии кнопки выход нужно отчищать память
			}
			window.draw(background_sprite);
			window.draw(menu_sprite_1);
			window.draw(menu_sprite_2);
			window.draw(menu_sprite_3);
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Arrow))
				window.setMouseCursor(cursor);

			window.display();
		}
	}

	void swap_check_pause()
	{
		check_pause = !check_pause;
	}
};