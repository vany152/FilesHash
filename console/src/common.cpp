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

/// вывод выделенного цветом исключения
void displayError(const string & what, const string & delim, Color color) {
	changeConsoleTextColor(color);
	cerr << what << delim;
	changeConsoleTextColor(white);
}

/** ************************************************** END ERRORS ************************************************** **/

/// изменяет цвет консоли
void changeConsoleTextColor(Color color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/// удаление последовательности символов с начала строки
void removeSymbolsFront(string & str, char symbol) {
	for (auto i = str.begin(); *i == symbol;) { str.erase(i); }
}

/// удаление последовательности символов с конца строки
void removeSymbolsBack(string & str, char symbol) {
	for (auto i = str.end() - 1; *i == symbol; --i) { str.erase(i); }
}

/// удаление последовательности символов с начала и конца строки
void removeSymbolsFrontBack(string & str, char symbol) {
	removeSymbolsFront(str, symbol);
	removeSymbolsBack(str, symbol);
}

/// ввод пути
fs::path inputPath() {
	cout << "введите путь: ";
	string pathStr;
	
	/*
	 * cin по какой-то причине в конце строки считывает "\n\n", когда был введен только один '\n',
	 * и при втором чтении getline не помещает в pathStr символы, так как наткнулся на последний '\n'.
	 * Для избежания этой ситуации был добавлен цикл while. Можно было бы использовать cin.ignore(),
	 * но в таком случае при первом чтении getline удаляет первый символ введенной строки.
	 */
	
	changeConsoleTextColor(green);
	while (pathStr.empty()) { getline(cin, pathStr); }
	changeConsoleTextColor(white);
	
	return {pathStr, locale("rus_rus.866")};
}

/// ввод выбора
string inputChoice() {
	
	/**
	 * при использовании конструкции cin >> choice или при представлении choice
	 * как целочисленной переменной возникает неотлавливаемое исключение
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

/// ввод количества потоков
int inputAmountOfThreads() {
	int amount;
	cout << "количество потоков (только для папок): ";
	
	changeConsoleTextColor(green);
	cin >> amount;
	changeConsoleTextColor(white);
	
	while (amount < 1) {
		cout << "количество потоков не может быть меньше 1\n"
		     << "количество потоков (только для папок): ";
		
		changeConsoleTextColor(green);
		cin >> amount;
		changeConsoleTextColor(white);
	}
	
	return amount;
}

/// вывод на экран размера файла или папки в формате ГБ-МБ-КБ.Б
void displaySize(uint64 bytes) {
	uint32_t kbytes = int(bytes / 1024) % 1024;
	uint32_t mbytes = int(bytes / 1048576) % 1024; // 1048576 = 1024^2
	uint32_t gbytes = int(bytes / 1073741824) % 1024; // 1073741824 = 1024^3
	
	if (gbytes != 0) { cout << gbytes << " гб "; }
	if (mbytes != 0) { cout << mbytes << " мб "; }
	if (kbytes != 0) { cout << kbytes << " кб "; }
	if (gbytes + mbytes + kbytes == 0) { cout << bytes << " б "; }
	
	cout << endl;
}

/// вычисление количества цифр числа
int numberLen(int num) {
	int len = 1;
	for (; num /= 10; ++len);
	
	return len;
}

/// изменение позиции курсора коммандной строки
void changeCursorPosition(short x, short y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, COORD{x, y});
}

/// установка видимости курсора консоли
void setCursorVisible(bool visible) {
	void * handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(handle, &cursor);
	cursor.bVisible = visible;
	SetConsoleCursorInfo(handle, &cursor);
}