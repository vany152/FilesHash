#ifndef UTILITIES_TIMER_H
#define UTILITIES_TIMER_H

#include <iostream>
#include <sstream>
#include <chrono>
#include <exception>
#include <windows.h>

#include "common.h"

using namespace std;

class Timer {
public:
	/// запуск таймера
	void Start();
	
	/// остановка таймера
	void Stop();
	
	/// преобразование секунд в строку формата ЧЧ-ММ-СС-МС
	string Duration();
	
	/// получение состояния таймера
	bool IsActive();

private:
	struct TimerError : Error {};
	
	chrono::time_point<chrono::steady_clock> startTime, stopTime; ///< время запуска и время остановки таймера
	bool active = false; ///< состояние таймера
	
	/// запуск таймера
	void start();
	
	/// остановка таймера
	void stop();
	
	/// вычисление временного интервала между стартом и остановкой таймера
	double duration();
	
	/// преобразование секунд в строку формата ЧЧ-ММ-СС-МС
	static string toString(double duration);
};


#endif //UTILITIES_TIMER_H
