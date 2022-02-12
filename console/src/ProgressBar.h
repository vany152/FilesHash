#ifndef PROGRESS_BAR_PROGRESSBAR_H
#define PROGRESS_BAR_PROGRESSBAR_H

#include <windows.h>
#include <string>
#include <cstdio>
#include <cmath>
#include <sstream>

#include "common.h"

using namespace std;

class ProgressBar {
public:
	ProgressBar(const double & startValue, const double & endValue, int x, int y);
	ProgressBar(const double & startValue, const double & endValue);
	~ProgressBar();
	
	/// установка границ строки прогресса
	void SetBorders(const double & startValue, const double & endValue);
	
	/// установка символа, отображающего пустое пространство строки прогресс
	void SetEmptyIncrementSymbol(char symbol);
	
	/// установка символа, отображающего полное пространство строки прогресса
	void SetFullIncrementSymbol(char symbol);
	
	/// установка символа, отображающих пустое и полное пространство строки прогресса
	void SetIncrementSymbols(char _emptyIncrementSymbol, char _fullIncrementSymbol);
	
	/// установка длины строки прогресса в символах
	void SetScaleLen(int len);
	
	/// установка позиции отрисовки левой границы строки прогресса
	void SetPosition(int x, int y);
	
	/// обновление строки прогресса
	void Update(const double & incrementValue);
	
	/// удаление из консоли строки прогресса
	void Clear();

private:
	struct ProgressBarError : Error {};
	
	int scaleLen = 20; ///< количество делений шкалы
	double incrementPerInterval;
	char emptyIncrementSymbol = '-'; ///< символ, отображающий пустое деление шкалы
	char fullIncrementSymbol = '#'; ///< символ, отображающий полное деление шкалы
	
	short coord_x = 0; ///< позиция X начала строки состояния
	short coord_y = 0; ///< позиция Y начала строки состояния
	
	double endValue; ///< конечное значение шкалы
	double currentValue; ///< текущее значение шкалы
	
	/// вывод в консоль пустой строки прогресса
	inline void printProgressBar() const;
	
	/// перерисовка символов в консоли
	inline void repaintSymbols(short startPositionX, short startPositionY, int amount, char symbol);
	
	/// обновление вывода процентов
	inline void updatePercents(const int & oldValue, const int & newValue);
	
	/// обновление строки прогресса
	inline void updateProgressBar(const double & incrementValue);
	
};


#endif //PROGRESS_BAR_PROGRESSBAR_H
