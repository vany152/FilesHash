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
	/// ������ �������
	void Start();
	
	/// ��������� �������
	void Stop();
	
	/// �������������� ������ � ������ ������� ��-��-��-��
	string Duration();
	
	/// ��������� ��������� �������
	bool IsActive();

private:
	struct TimerError : Error {};
	
	chrono::time_point<chrono::steady_clock> startTime, stopTime; ///< ����� ������� � ����� ��������� �������
	bool active = false; ///< ��������� �������
	
	/// ������ �������
	void start();
	
	/// ��������� �������
	void stop();
	
	/// ���������� ���������� ��������� ����� ������� � ���������� �������
	double duration();
	
	/// �������������� ������ � ������ ������� ��-��-��-��
	static string toString(double duration);
};


#endif //UTILITIES_TIMER_H
