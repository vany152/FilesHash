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

/// ������� ����� ��� ���������������� � ������ ��������� ������
struct Error : public exception {
	string content;
	string delim = "\n\n";
	Color color = red;
	
	void Set(const char * what, const char delim[], Color color);
	void Set(const char * what);
	
	void What();
};

ostream & operator<<(ostream & out, const Error & err);

/// ����� ����������� ������ ����������
void displayError(const string & what, const string & delim = "\n\n", Color color = red);

/// �������� ���� �������
void changeConsoleTextColor(Color color);

/// �������� ������������������ �������� � ������ ������
void removeSymbolsFront(string & str, char symbol = ' ');

/// �������� ������������������ �������� � ����� ������
void removeSymbolsBack(string & str, char symbol = ' ');

/// /// �������� ������������������ �������� � ������ � ����� ������
void removeSymbolsFrontBack(string & str, char symbol = ' ');

/// ���� ����
fs::path inputPath();

/// ���� ������
string inputChoice();

/// ���� ���������� �������
int inputAmountOfThreads();

/// ����� �� ����� ������� ����� ��� ����� � ������� ��-��-��.�
void displaySize(uint64 bytes);

/// ���������� ���������� ���� �����
int numberLen(int num);

/// ��������� ������� ������� ���������� ������
void changeCursorPosition(short x, short y);

/// ��������� ��������� ������� �������
void setCursorVisible(bool visible);

#endif //UTILITIES_COMMON_H
