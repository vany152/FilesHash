#include "Timer.h"

/** *********************************************** PUBLIC SECTION ************************************************ **/

/// ������ �������
void Timer::Start() {
	try {
		start();
		
	} catch (TimerError & err) {
		err.What();
		system("pause");
		exit(EXIT_FAILURE);
	}
}

/// ��������� �������
void Timer::Stop() {
	try {
		stop();
		
	} catch (TimerError & err) {
		err.What();
		system("pause");
		exit(EXIT_FAILURE);
	}
}

/// �������������� ������ � ������ ������� ��-��-��-��
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

/// ��������� ��������� �������
bool Timer::IsActive() {
	return active;
}

/** *********************************************** PRIVATE SECTION *********************************************** **/

/// ������ �������
void Timer::start() {
	if (active) {
		TimerError err;
		err.Set((char *) "������ ��� �������");
		throw err;
	}
	
	startTime = chrono::steady_clock::now();
	active = true;
}

/// ��������� �������
void Timer::stop() {
	if (!active) {
		TimerError err;
		err.Set((char *) "������ ��������� (�� ������� ��� ����������)");
		throw err;
	}
	
	stopTime = chrono::steady_clock::now();
	active = false;
}

/// ���������� ���������� ��������� ����� ������� � ���������� �������
double Timer::duration() {
	if (active) {
		TimerError err;
		err.Set((char *) "������ �������� ��������� �������� ��������� �������");
		throw err;
	}
	
	return chrono::duration<double>(stopTime - startTime).count();
}

/// �������������� ������ � ������ ������� ��-��-��-��
string Timer::toString(double duration) {
	/**
	 * ����������, ������ ����, �� ���������.
	 * ���� ��� ���������� ����� ����, ��������� "0��"
	 */
	
	ostringstream result;
	
	int _duration = (int) duration;
	uint32_t milliseconds = int(duration * 1000) % 1000;
	uint32_t seconds = _duration % 60;
	uint32_t minutes = _duration % 3600 / 60;
	uint32_t hours = _duration / 3600;
	
	if (hours != 0) { result << hours << " �  "; }
	if (minutes != 0) { result << minutes << " �  "; }
	if (seconds != 0) { result << seconds << " �  "; }
	if (milliseconds != 0) { result << milliseconds << " ��"; }
	
	if (hours + minutes + seconds + milliseconds == 0) { result << "0��"; }
	
	return result.str();
}