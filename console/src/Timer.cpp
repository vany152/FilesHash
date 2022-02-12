#include "Timer.h"

/** *********************************************** PUBLIC SECTION ************************************************ **/

/// запуск таймера
void Timer::Start() {
	try {
		start();
		
	} catch (TimerError & err) {
		err.What();
		system("pause");
		exit(EXIT_FAILURE);
	}
}

/// остановка таймера
void Timer::Stop() {
	try {
		stop();
		
	} catch (TimerError & err) {
		err.What();
		system("pause");
		exit(EXIT_FAILURE);
	}
}

/// преобразование секунд в строку формата ЧЧ-ММ-СС-МС
string Timer::Duration() {
	double _duration;
	try {
		_duration = duration();
		
	} catch (TimerError & err) {
		err.What();
		
		system("pause");
		exit(EXIT_FAILURE);
	}
	
	return toString(_duration);
}

/// получение состояния таймера
bool Timer::IsActive() {
	return active;
}

/** *********************************************** PRIVATE SECTION *********************************************** **/

/// запуск таймера
void Timer::start() {
	if (active) {
		TimerError err;
		err.Set((char *) "таймер уже активен");
		throw err;
	}
	
	startTime = chrono::steady_clock::now();
	active = true;
}

/// остановка таймера
void Timer::stop() {
	if (!active) {
		TimerError err;
		err.Set((char *) "таймер неактивен (не запущен или остановлен)");
		throw err;
	}
	
	stopTime = chrono::steady_clock::now();
	active = false;
}

/// вычисление временного интервала между стартом и остановкой таймера
double Timer::duration() {
	if (active) {
		TimerError err;
		err.Set((char *) "нельзя измерить временной интервал активного таймера");
		throw err;
	}
	
	return chrono::duration<double>(stopTime - startTime).count();
}

/// преобразование секунд в строку формата ЧЧ-ММ-СС-МС
string Timer::toString(double duration) {
	/**
	 * компоненты, равные нулю, не выводятся.
	 * если все компоненты равны нулю, выводится "0мс"
	 */
	
	ostringstream result;
	
	int _duration = (int) duration;
	uint32_t milliseconds = int(duration * 1000) % 1000;
	uint32_t seconds = _duration % 60;
	uint32_t minutes = _duration % 3600 / 60;
	uint32_t hours = _duration / 3600;
	
	if (hours != 0) { result << hours << " ч  "; }
	if (minutes != 0) { result << minutes << " м  "; }
	if (seconds != 0) { result << seconds << " с  "; }
	if (milliseconds != 0) { result << milliseconds << " мс"; }
	
	if (hours + minutes + seconds + milliseconds == 0) { result << "0мс"; }
	
	return result.str();
}