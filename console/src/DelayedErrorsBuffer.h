#ifndef UTILITIES_DELAYEDERRORSBUFFER_H
#define UTILITIES_DELAYEDERRORSBUFFER_H

#include "common.h"

using namespace std;

class DelayedErrorsBuffer {
public:
	~DelayedErrorsBuffer();
	
	/// добавление текста ошибки в буфер
	void AddErrorToBuffer(const Error & err);
	
	/// вывод буфера на экран
	void OutputBuffer();

private:
	list<Error> errorsWithDelayedOutput; ///< ошибки с отложенным выводом
	
	/// добавление текста ошибки в буфер
	void addErrorToBuffer(const Error & err);
	
	/// вывод буфера на экран
	void outputErrorsBuffer();
	
	/// очистка буфера
	void clearErrorsBuffer();
};


#endif //UTILITIES_DELAYEDERRORSBUFFER_H
