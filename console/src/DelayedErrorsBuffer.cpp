#include "DelayedErrorsBuffer.h"

/** *********************************************** PUBLIC SECTION ************************************************ **/

DelayedErrorsBuffer::~DelayedErrorsBuffer() {
	OutputBuffer();
}

/// ���������� ������ ������ � �����
void DelayedErrorsBuffer::AddErrorToBuffer(const Error & err) {
	addErrorToBuffer(err);
}

/// ����� ������ �� �����
void DelayedErrorsBuffer::OutputBuffer() {
	outputErrorsBuffer();
	clearErrorsBuffer();
}

/** *********************************************** PRIVATE SECTION *********************************************** **/

/// ���������� ������ ������ � �����
void DelayedErrorsBuffer::addErrorToBuffer(const Error & err) {
	errorsWithDelayedOutput.push_back(err);
}

/// ����� ������ �� �����
void DelayedErrorsBuffer::outputErrorsBuffer() {
	for (auto & err : errorsWithDelayedOutput) { cerr << err; }
}

/// ������� ������
void DelayedErrorsBuffer::clearErrorsBuffer() {
	errorsWithDelayedOutput.clear();
}


