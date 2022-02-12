#include "ProgressBar.h"

/** *********************************************** PUBLIC SECTION ************************************************ **/

ProgressBar::ProgressBar(const double & _startValue, const double & _endValue, int x, int y) {
	SetBorders(_startValue, _endValue);
	SetPosition(x, y);
	printProgressBar();
}

ProgressBar::ProgressBar(const double & _startValue, const double & _endValue) {
	SetBorders(_startValue, _endValue);
}

ProgressBar::~ProgressBar() {
	Sleep(500);
	cout << endl;

	setCursorVisible(true);
}

/// установка границ строки прогресса
void ProgressBar::SetBorders(const double & _startValue, const double & _endValue) {
	
	/**
	 * нижняя граница всегда 0, а верхняя равняется разности поступивших верхней и нижней границ
	 */
	
	if (_startValue - _endValue >= 0.0000000000001) {
		ProgressBarError err;
		err.Set(string("начальное значение шкалы должно быть меньше конечного: " +
		               to_string(_startValue) + " и " + to_string(_endValue)).c_str());
		throw err;
	}
	
	this->endValue = _endValue - _startValue;
	this->incrementPerInterval = endValue / scaleLen;
}

/// установка символа, отображающего пустое пространство строки прогресса
void ProgressBar::SetEmptyIncrementSymbol(char symbol) {
	if (symbol == fullIncrementSymbol) {
		ProgressBarError err;
		err.Set(string("символы пустого и полного деления должны различаться: " +
		               string(&symbol) + " == " + string(&fullIncrementSymbol)).c_str());
		throw err;
	}
	
	emptyIncrementSymbol = symbol;
}

/// установка символа, отображающего полное пространство строки прогресса
void ProgressBar::SetFullIncrementSymbol(char symbol) {
	if (symbol == emptyIncrementSymbol) {
		ProgressBarError err;
		err.Set(string("символы пустого и полного деления должны различаться: " +
		               string(&emptyIncrementSymbol) + " == " + string(&symbol)).c_str());
		throw err;
	}
	
	fullIncrementSymbol = symbol;
}

/// установка символа, отображающих пустое и полное пространство строки прогресс
void ProgressBar::SetIncrementSymbols(char _emptyIncrementSymbol, char _fullIncrementSymbol) {
	if (_emptyIncrementSymbol == _fullIncrementSymbol) {
		ProgressBarError err;
		err.Set(string("символы пустого и полного деления должны различаться: " +
		               string(&_emptyIncrementSymbol) + " == " + string(&_fullIncrementSymbol)).c_str());
		throw err;
	}
	
	SetEmptyIncrementSymbol(_emptyIncrementSymbol);
	SetFullIncrementSymbol(_fullIncrementSymbol);
}

/// установка длины строки прогресса в символах
void ProgressBar::SetScaleLen(int len) {
	scaleLen = len;
	incrementPerInterval = 100.0 / scaleLen;
}

/// установка позиции отрисовки левой границы строки прогресса
void ProgressBar::SetPosition(int x, int y) {
	if (x < 0 || y < 0) {
		ProgressBarError err;
		err.Set("координаты курсора не могут быть отрицательными");
		throw err;
	}
	
	this->coord_x = short(x);
	this->coord_y = short(y);
}

/// обновление строки прогресса
void ProgressBar::Update(const double & incrementValue) {
	updateProgressBar(incrementValue);
}

/// удаление из консоли строки прогресса
void ProgressBar::Clear() {
	changeCursorPosition(coord_x, coord_y);
	
	int len = scaleLen + 10; // +2 - скобки шкалы, +3 - интервал до процентов, +5 - проценты
	for (int i = 0; i < len; ++i) { putchar(' '); }
	
	putchar('\r');
}

/** *********************************************** PRIVATE SECTION *********************************************** **/

/// вывод в консоль пустой строки прогресса
void ProgressBar::printProgressBar() const {
	setCursorVisible(false);
	
	changeCursorPosition(coord_x, coord_y);
	
	int amountOfFullIncrementSymbol = int(round(currentValue / incrementPerInterval)) % (scaleLen + 1);
	
	putchar('[');
	for (int i = 0; i < amountOfFullIncrementSymbol; ++i) { putchar(fullIncrementSymbol); }
	for (int i = scaleLen - amountOfFullIncrementSymbol; i > 0; --i) { putchar(emptyIncrementSymbol); }
	putchar(']');
	
	printf("   %s", "0 %");
}

/// перерисовка символов в консоли
void ProgressBar::repaintSymbols(short startPositionX, short startPositionY, int amount, char symbol) {
	changeCursorPosition(startPositionX, startPositionY);
	
	for (int i = 0; i < amount; ++i) { putchar(symbol); }
}

/// обновление вывода процентов
void ProgressBar::updatePercents(const int & oldValue, const int & newValue) {
	repaintSymbols(coord_x + scaleLen + 5, coord_y, numberLen(oldValue) + 2, ' ');
	
	changeCursorPosition(short(coord_x + scaleLen + 5), coord_y);
	printf("%d %c", newValue, '%');
}

/// обновление строки прогресса
void ProgressBar::updateProgressBar(const double & incrementValue) {
	int oldPercents = int(currentValue / endValue * 100);
	short amountOfFullIncrementSymbol = short(round(currentValue / incrementPerInterval)) % (scaleLen + 1);
	
	currentValue += incrementValue;
	if (currentValue < 0) { currentValue = 0; }
	else if (currentValue > endValue) { currentValue = endValue; }
	
	int newPercents = int(currentValue / endValue * 100);
	
	short newAmountOfFullIntervals = short(abs(round(currentValue / incrementPerInterval))) % (scaleLen + 1);
	
	short startRepaintX;
	int amountOfSymbolsToRepaint;
	
	if (amountOfFullIncrementSymbol != newAmountOfFullIntervals) {
		if (incrementValue > 0) {
			startRepaintX = amountOfFullIncrementSymbol + 1; // +1, чтобы учесть открывающую скобку
			amountOfSymbolsToRepaint = newAmountOfFullIntervals - amountOfFullIncrementSymbol;
			repaintSymbols(startRepaintX, coord_y, amountOfSymbolsToRepaint, fullIncrementSymbol);
			
		} else {
			startRepaintX = newAmountOfFullIntervals + 1; // +1, чтобы учесть открывающую скобку
			amountOfSymbolsToRepaint = amountOfFullIncrementSymbol - newAmountOfFullIntervals;
			repaintSymbols(startRepaintX, coord_y, amountOfSymbolsToRepaint, emptyIncrementSymbol);
		}
	}
	
	if (oldPercents != newPercents) {
		updatePercents(oldPercents, newPercents);
	}
}


