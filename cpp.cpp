#include <iostream>
#include <stdio.h>
using namespace std;

#ifdef __linux__ // linux moment
	#include <stdlib.h>
	#define ENTER_CHAR "\r\n"
	void showMessageBox(string message) {
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
	


// const int ENTER = 13;
// const int ESCAPE = 27;
 
int main()
{
	// linux moment
#ifdef __linux__
	system ("/bin/stty raw");
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
 
	// таблица аски для поиска символов псевдографики
	//for (int code = 0; code < 256; code++)
	//{
	//	cout << code << " - " << (char)code << "\n";
	//}
 
	// дескриптор окна консоли (порядковый номер окна в системе)
	// HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	// скрыть стандартный мигающий курсор
	// Windows:
	// CONSOLE_CURSOR_INFO info;
	// info.bVisible = false;
	// info.dwSize = 100;
	// SetConsoleCursorInfo(h, &info);
	// cross-platform
	cout << "\033[?25l";
 
	const int WIDTH = 50;
	const int HEIGHT = 15;
	int location[HEIGHT][WIDTH] = {};
 
	// модель локации
	// 0 - коридоры (пустоты)
	// 1 - стена разрушаемая
	// 2 - монетки
	// 3 - враги
 
	// генерация локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			// по дефолту пишется случайное число
			location[y][x] = rand() % 5; // 0 1 2 3 4
 
			// стены по краям
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = WALL;
 
			// вход и выход
			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = HALL;
 
			if (location[y][x] == ENEMY || location[y][x] == FIRST_AID) {
				// определяется вероятность того, останется враг или нет
				// допустим, вероястность остаться на уровне - 10%
				int prob = rand() % 10; // 0-9
				if (prob != 0) // 1 2 3 4 5 6 7 8 9
					location[y][x] = HALL;
			}
		}
	}
 
	// показ локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// cout << location[y][x];
			switch (location[y][x]) {
			case HALL: // коридор
				cout << " ";
				break;
			case WALL: // стена
				//SetConsoleTextAttribute(h, DARKGREEN); // 0-255
				cout << DARKGREEN;
				cout << '#'; //(char)219;
				break;
			case COIN: // монетки
				//SetConsoleTextAttribute(h, YELLOW);
				cout << YELLOW;
				cout << '@'; // 249
				cCoins++;
				break;
			case ENEMY: // враги
				//SetConsoleTextAttribute(h, RED);
				cout << RED;
				cout << 'X';
				break;
			case FIRST_AID:
				cout << GREEN;
				cout << '+';
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << ENTER_CHAR;
	}

	// строка для ввода(getchar оставляет букву после себя, поэтому добавлю чёрную строку где всё будет написано чёрным текстом)
	for (int x = 0; x < WIDTH; x++){
		cout << " ";
	}

	// размещение ГГ (главного героя)
	// COORD position;
	// position.X = 1;
	// position.Y = 2;
	int position[2] = {2, 0}; // y,x

	// функция установки курсора в любую точку на экране консоли
	// SetConsoleCursorPosition(h, position);
	// SetConsoleTextAttribute(h, BLUE);
	cout << "\033[" << position[0] + 1 << ";" << position[1] + 1 << "H";
	cout << BLUE;
	cout << 'O';
	
	cout << "\033["<< HEIGHT + 1 << ";1H";
	cout << YELLOW;
	cout << "Coins: 0";

	
	cout << "\033["<< HEIGHT + 1 << ";" << WIDTH - 10 <<"H";
	cout << RED;
	cout << "Health:OOO";
	// игровой движок (интерактив с пользователем)
	while (true) {
		cout << "\033["<< HEIGHT + 1 << ";21H";
		cout << BLACK;
		code = getchar();
		// стирание ГГ в старой позиции
		// SetConsoleCursorPosition(h, position);
		// SetConsoleTextAttribute(h, BLUE);c

		cout << "\033[" << position[0] + 1 << ";" << position[1] + 1 << "H";
		cout << BLUE;
		cout << " ";
 
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
			// cout << "COIN COLLECTED!\n";
			location[position[0]][position[1]] = HALL;
			cout << "\033[" << position[0] + 1 << ";" << position[1] + 1 << "H";
			cout << " ";
			cout << "\033["<< HEIGHT + 1 << ";8H";
			cout << YELLOW;
			cout << coins;
			if (coins >= cCoins){
				showMessageBox("You Won!");
			}
		} else if (location[position[0]][position[1]] == ENEMY){
			health--;
			location[position[0]][position[1]] = HALL;
			cout << "\033["<< HEIGHT + 1 << ";" << WIDTH - 3 << "H";
			switch (health){
			case 6:
				cout << RED;
				cout << "OOO";
				break;
			case 5:
				cout << RED;
				cout << "OOo";
				break;
			case 4:
				cout << RED;
				cout << "OO";
				cout << GREY;
				cout << "o";
				break;
			case 3:
				cout << RED;
				cout << "Oo";
				cout << GREY;
				cout << "o";
				break;
			case 2:
				cout << RED;
				cout << "O";
				cout << GREY;
				cout << "oo";
				break;
			case 1:
				cout << RED;
				cout << "o";
				cout << GREY;
				cout << "oo";
				break;
			case 0:
				cout << GREY;
				cout << "ooo";
				showMessageBox("You Lost!");
				return 0;
				break;
			default:
				break;
			}
		} else if (location[position[0]][position[1]] == FIRST_AID){
			if (health <= 5){
				health++;
			}
			location[position[0]][position[1]] = HALL;
			cout << "\033["<< HEIGHT + 1 << ";" << WIDTH - 3 << "H";
			switch (health){
			case 6:
				cout << RED;
				cout << "OOO";
				break;
			case 5:
				cout << RED;
				cout << "OOo";
				break;
			case 4:
				cout << RED;
				cout << "OO";
				cout << GREY;
				cout << "o";
				break;
			case 3:
				cout << RED;
				cout << "Oo";
				cout << GREY;
				cout << "o";
				break;
			case 2:
				cout << RED;
				cout << "O";
				cout << GREY;
				cout << "oo";
				break;
			case 1:
				cout << RED;
				cout << "o";
				cout << GREY;
				cout << "oo";
				break;
			case 0:
				cout << GREY;
				cout << "ooo";
				showMessageBox("You Lost!");
				return 0;
				break;
			default:
				break;
			}
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