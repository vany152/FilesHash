#include "DelayedErrorsBuffer.h"

/** *********************************************** PUBLIC SECTION ************************************************ **/

DelayedErrorsBuffer::~DelayedErrorsBuffer() {
	OutputBuffer();
}

/// добавление текста ошибки в буфер
void DelayedErrorsBuffer::AddErrorToBuffer(const Error & err) {
	addErrorToBuffer(err);
}

/// вывод буфера на экран
void DelayedErrorsBuffer::OutputBuffer() {
	outputErrorsBuffer();
	clearErrorsBuffer();
}

/** *********************************************** PRIVATE SECTION *********************************************** **/

/// добавление текста ошибки в буфер
void DelayedErrorsBuffer::addErrorToBuffer(const Error & err) {
	errorsWithDelayedOutput.push_back(err);
}

/// вывод буфера на экран
void DelayedErrorsBuffer::outputErrorsBuffer() {
	for (auto & err : errorsWithDelayedOutput) { cerr << err; }
}

/// очистка буфера
void DelayedErrorsBuffer::clearErrorsBuffer() {
	errorsWithDelayedOutput.clear();
}


