#ifndef UTILITIES_COMMON_H
#define UTILITIES_COMMON_H

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <list>

using namespace std;
namespace fs = std::filesystem;
using uint64 = unsigned long long;
using uint32_t = unsigned int;

enum Color {
	red = FOREGROUND_RED,
	green = FOREGROUND_GREEN,
	blue = FOREGROUND_BLUE,
	yellow = FOREGROUND_RED | FOREGROUND_GREEN,
	purple = FOREGROUND_RED | FOREGROUND_BLUE,
	cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

/// базовый класс для переопределенных в данной программе ошибок
struct Error : public exception {
	string content;
	string delim = "\n\n";
	Color color = red;
	
	void Set(const char * what, const char delim[], Color color);
	void Set(const char * what);
	
	void What();
};

ostream & operator<<(ostream & out, const Error & err);

/// вывод выделенного цветом исключения
void displayError(const string & what, const string & delim = "\n\n", Color color = red);

/// изменяет цвет консоли
void changeConsoleTextColor(Color color);

/// удаление последовательности символов с начала строки
void removeSymbolsFront(string & str, char symbol = ' ');

/// удаление последовательности символов с конца строки
void removeSymbolsBack(string & str, char symbol = ' ');

/// /// удаление последовательности символов с начала и конца строки
void removeSymbolsFrontBack(string & str, char symbol = ' ');

/// ввод пути
fs::path inputPath();

/// ввод выбора
string inputChoice();

/// ввод количества потоков
int inputAmountOfThreads();

/// вывод на экран размера файла или папки в формате ГБ-МБ-КБ.Б
void displaySize(uint64 bytes);

/// вычисление количества цифр числа
int numberLen(int num);

/// изменение позиции курсора коммандной строки
void changeCursorPosition(short x, short y);

/// установка видимости курсора консоли
void setCursorVisible(bool visible);

#endif //UTILITIES_COMMON_H
