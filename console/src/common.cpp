#include "common.h"

/** **************************************************** ERRORS **************************************************** **/

void Error::Set(const char * what, const char _delim[], Color _color) {
	this->content = string(what);
	this->delim = string(_delim);
	this->color = _color;
}

void Error::Set(const char * what) {
	this->content = string(what);
}

void Error::What() {
	displayError(content, delim, color);
}

ostream & operator<<(ostream & out, const Error & err) {
	changeConsoleTextColor(err.color);
	out << err.content << err.delim;
	changeConsoleTextColor(white);
	
	return out;
}

/// ����� ����������� ������ ����������
void displayError(const string & what, const string & delim, Color color) {
	changeConsoleTextColor(color);
	cerr << what << delim;
	changeConsoleTextColor(white);
}

/** ************************************************** END ERRORS ************************************************** **/

/// �������� ���� �������
void changeConsoleTextColor(Color color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/// �������� ������������������ �������� � ������ ������
void removeSymbolsFront(string & str, char symbol) {
	for (auto i = str.begin(); *i == symbol;) { str.erase(i); }
}

/// �������� ������������������ �������� � ����� ������
void removeSymbolsBack(string & str, char symbol) {
	for (auto i = str.end() - 1; *i == symbol; --i) { str.erase(i); }
}

/// �������� ������������������ �������� � ������ � ����� ������
void removeSymbolsFrontBack(string & str, char symbol) {
	removeSymbolsFront(str, symbol);
	removeSymbolsBack(str, symbol);
}

/// ���� ����
fs::path inputPath() {
	cout << "������� ����: ";
	string pathStr;
	
	/*
	 * cin �� �����-�� ������� � ����� ������ ��������� "\n\n", ����� ��� ������ ������ ���� '\n',
	 * � ��� ������ ������ getline �� �������� � pathStr �������, ��� ��� ��������� �� ��������� '\n'.
	 * ��� ��������� ���� �������� ��� �������� ���� while. ����� ���� �� ������������ cin.ignore(),
	 * �� � ����� ������ ��� ������ ������ getline ������� ������ ������ ��������� ������.
	 */
	
	changeConsoleTextColor(green);
	while (pathStr.empty()) { getline(cin, pathStr); }
	changeConsoleTextColor(white);
	
	return {pathStr, locale("rus_rus.866")};
}

/// ���� ������
string inputChoice() {
	
	/**
	 * ��� ������������� ����������� cin >> choice ��� ��� ������������� choice
	 * ��� ������������� ���������� ��������� ��������������� ����������
	 */
	
	string choice;
	do {
		cout << "(y / n): ";
		
		changeConsoleTextColor(green);
		cin.ignore();
		getline(cin, choice, '\n');
		changeConsoleTextColor(white);
		
		removeSymbolsFrontBack(choice);
		
	} while (choice != "y" && choice != "n");
	
	return choice;
}

/// ���� ���������� �������
int inputAmountOfThreads() {
	int amount;
	cout << "���������� ������� (������ ��� �����): ";
	
	changeConsoleTextColor(green);
	cin >> amount;
	changeConsoleTextColor(white);
	
	while (amount < 1) {
		cout << "���������� ������� �� ����� ���� ������ 1\n"
		     << "���������� ������� (������ ��� �����): ";
		
		changeConsoleTextColor(green);
		cin >> amount;
		changeConsoleTextColor(white);
	}
	
	return amount;
}

/// ����� �� ����� ������� ����� ��� ����� � ������� ��-��-��.�
void displaySize(uint64 bytes) {
	uint32_t kbytes = int(bytes / 1024) % 1024;
	uint32_t mbytes = int(bytes / 1048576) % 1024; // 1048576 = 1024^2
	uint32_t gbytes = int(bytes / 1073741824) % 1024; // 1073741824 = 1024^3
	
	if (gbytes != 0) { cout << gbytes << " �� "; }
	if (mbytes != 0) { cout << mbytes << " �� "; }
	if (kbytes != 0) { cout << kbytes << " �� "; }
	if (gbytes + mbytes + kbytes == 0) { cout << bytes << " � "; }
	
	cout << endl;
}

/// ���������� ���������� ���� �����
int numberLen(int num) {
	int len = 1;
	for (; num /= 10; ++len);
	
	return len;
}

/// ��������� ������� ������� ���������� ������
void changeCursorPosition(short x, short y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, COORD{x, y});
}

/// ��������� ��������� ������� �������
void setCursorVisible(bool visible) {
	void * handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(handle, &cursor);
	cursor.bVisible = visible;
	SetConsoleCursorInfo(handle, &cursor);
}