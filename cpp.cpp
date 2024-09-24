#include <iostream>
#include <stdio.h>
using namespace std;

#ifdef __linux__ // linux moment
	#include <stdlib.h>
	#define ENTER_CHAR "\r\n"
	void showMessageBox(string message = " ") {
        string command = "zenity --info --text=\"" + message + "\"";
        system(command.c_str());
    }
	enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 97, RIGHT = 100, UP = 119, DOWN = 115, SPACEBAR = 32 };
#else
	#define ENTER_CHAR "\n"
	#include <windows.h>  
    void showMessageBox(string message) {
        MessageBox(NULL, message.c_str(), "Victory!", MB_OK | MB_ICONINFORMATION); // код взял с интернета т.к. не могу проверить как он работает напрямую
    }
	enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
#endif

enum Objects { HALL, WALL, COIN, ENEMY, FIRST_AID };

// использую ANSI Escape Sequences
// enum не работает со стрингом, использую define как альтернативу
#define RESET "\033[0m" // установить стандартный цвет
#define DARKGREEN "\033[38;5;22m"
#define RED "\033[38;5;160m"
#define YELLOW "\033[38;5;226m"
#define BLUE "\033[38;5;33m"
#define BLACK "\033[38;2;1;1;1m"
#define GREY "\033[38;5;8m"
#define GREEN "\033[38;5;40m"

void myExit();
void updateHearts(int &health, bool enemy = true, int height = 15, int width = 40);
void showElement(string str = "", string color = " ", int y = 0, int x = 0);
int Tile(int height = 15, int width = 40, int x = 0, int y = 0);
void updateTile(int x = 0, int y = 2, int type = HALL);

// const int ENTER = 13;
// const int ESCAPE = 27;
 
int main()
{
	// linux moment
#ifdef __linux__
	system("/bin/stty raw");
#endif
	int health = 6;
	int code;
	// Black background
	cout << "\033[1;40m";
	// установить курсор в исходное положение (не знаю как в Визуал студио но у меня начинало со 2 строки)
	cout << "\033[1;1H";
	int cCoins,coins = 0;

	// запуск алгоритма генерации случайных чисел
	srand(time(0));
	rand();

	// скрыть стандартный мигающий курсор
	cout << "\033[?25l";
	const int WIDTH = 50;
	const int HEIGHT = 15;
	int location[HEIGHT][WIDTH] = {};
 
	// модель локации
	// 0 - коридоры (пустоты)
	// 1 - стена разрушаемая
	// 2 - монетки
	// 3 - враги
	// 4 - аптечка
 
	// генерация и показ локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			location[y][x] = Tile(HEIGHT, WIDTH, x, y);
			updateTile(x, y, location[y][x]);
			if (location[y][x] == COIN){cCoins++;}
		}
	}
	cout << ENTER_CHAR;
	// строка для ввода(getchar оставляет букву после себя, поэтому добавлю чёрную строку где всё будет написано чёрным текстом)
	for (int x = 0; x < WIDTH; x++){
		cout << " ";
	}

	// размещение ГГ (главного героя)
	int position[2] = {2, 0}; // y,x

	// функция установки курсора в любую точку на экране консоли
	showElement("O", BLUE, position[0], position[1]);
	showElement("Coins: 0", YELLOW, HEIGHT, 0);
	showElement("Health: OOO", RED, HEIGHT, WIDTH-12);
	// игровой движок (интерактив с пользователем)
	while (true) {
		showElement(" ", BLACK, HEIGHT, 20);
		code = getchar();
		// стирание ГГ в старой позиции
		updateTile(position[1], position[0], location[position[0]][position[1]]);
 
		// пользователь может нажать любую кнопку (в том числе энтер, эскейп, пробел, стрелочки), после чего вернётся код нажатой клавиши
		switch (code) {
		case ENTER:
			// cout << "ENTER\n";
			break;
		case SPACEBAR:
			// cout << "SPACEBAR\n";
			break;
		case ESCAPE:
			// cout << "ESCAPE\n";
			break;
		case RIGHT:
			if (location[position[0]][position[1] + 1] != WALL)
				position[1]++;
			break;
		case LEFT:
			if (position[1] > 0 && location[position[0]][position[1] - 1] != WALL)
				position[1]--;
			break;
		case UP:
			if (location[position[0] - 1][position[1]] != WALL)
				position[0]--;
			break;
		case DOWN:
		if (location[position[0] + 1][position[1]] != WALL)
				position[0]++;
			break;
		default:
			//cout << code << "\n";
			break;
		}

		// взаимодействие ГГ с другими объектами в лабиринте
		if (location[position[0]][position[1]] == COIN) {
			coins++;
			location[position[0]][position[1]] = HALL;
			showElement(to_string(coins), YELLOW, HEIGHT, 8);

			if (coins >= cCoins){
				showMessageBox("You Won!");
			}

		} else if (location[position[0]][position[1]] == ENEMY){
			location[position[0]][position[1]] = HALL;
			updateHearts(health, true, HEIGHT, WIDTH);

		} else if (location[position[0]][position[1]] == FIRST_AID){

			if (health <= 5){location[position[0]][position[1]] = HALL;}
			updateHearts(health, false, HEIGHT, WIDTH);
		}

		// показ ГГ в новой позиции
		// SetConsoleCursorPosition(h, position);
		// SetConsoleTextAttribute(h, BLUE);
		cout << BLUE;
		cout << "\033[" << position[0] + 1 << ";" << position[1] + 1 << "H";
		cout << 'O';

	}
	// linux moment
#ifdef __linux__
	system ("/bin/stty cooked");
#endif
	return 0;
}

void showElement(string str, string color, int y, int x){
	cout << "\033[" << y+1 << ";" << x+1 << "H";
	if (color != " ") {cout << color;}
	cout << str;
}

int Tile(int height, int width, int x, int y){
	int location, temp;
	int probab[][3] = {
		{HALL, 1, 1},
		{WALL, 1, 1},
		{COIN, 2, 1},
		{ENEMY, 8, 1},
		{FIRST_AID, 10, 1}
	};
	location = rand() % 5;
	temp = rand() % probab[location][1];
	if (temp >= probab[location][2]){location = HALL;}
	if (!x || !y || x == width-1 || y == height-1){
		location = WALL;
		if (x == 0 && y == 2 || x == width - 1 && y == height - 3)
			location = HALL;
	}
	return location;
}

void updateTile(int x, int y, int type){
	switch (type) {
		case HALL: // коридор
			showElement(" ", BLACK, y, x);
			break;
		case WALL: // стена
			//SetConsoleTextAttribute(h, DARKGREEN); // 0-255
			showElement("#", DARKGREEN, y, x);
			break;
		case COIN: // монетки
			//SetConsoleTextAttribute(h, YELLOW);
			showElement("@", YELLOW, y, x);
			break;
		case ENEMY: // враги
			//SetConsoleTextAttribute(h, RED);
			showElement("X", RED, y, x);
			break;
		case FIRST_AID:
			showElement("+", GREEN, y, x);
			break;
		default:
			break;
	}
}

void updateHearts(int &health, bool enemy, int height, int width){
	if (!enemy){
		if (health < 6){
			health++;
		}
	} else{
		health--;
	}
	switch (health){
		case 6:
			showElement("OOO", RED, height, width - 4);
			break;
		case 5:
			showElement("OOo", RED, height, width - 4);
			break;
		case 4:
			showElement("OO", RED, height, width - 4);
			showElement("o", GREY, height, width - 2);
			break;
		case 3:
			showElement("Oo", RED, height, width - 4);
			showElement("o", GREY, height, width - 2);
			break;
		case 2:
			showElement("O", RED, height, width - 4);
			showElement("oo", GREY, height, width - 3);
			break;
		case 1:
			showElement("o", RED, height, width - 4);
			showElement("oo", GREY, height, width - 3);
			break;
		case 0:
			showElement("ooo", GREY, height, width - 4);
			showMessageBox("You Lost!");
			myExit();
			break;
		default:
			break;
	}
}

void myExit(){
	cout << ENTER_CHAR;
#ifdef __linux__
	system ("/bin/stty cooked");
#endif
	exit(0);
}