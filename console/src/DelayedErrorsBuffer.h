#ifndef UTILITIES_DELAYEDERRORSBUFFER_H
#define UTILITIES_DELAYEDERRORSBUFFER_H

#include "common.h"

using namespace std;

class DelayedErrorsBuffer {
public:
	~DelayedErrorsBuffer();
	
	/// ���������� ������ ������ � �����
	void AddErrorToBuffer(const Error & err);
	
	/// ����� ������ �� �����
	void OutputBuffer();

private:
	list<Error> errorsWithDelayedOutput; ///< ������ � ���������� �������
	
	/// ���������� ������ ������ � �����
	void addErrorToBuffer(const Error & err);
	
	/// ����� ������ �� �����
	void outputErrorsBuffer();
	
	/// ������� ������
	void clearErrorsBuffer();
};


#endif //UTILITIES_DELAYEDERRORSBUFFER_H
