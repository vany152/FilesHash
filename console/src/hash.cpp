#include "hash.h"

#define BYTES_TO_READ 32

/// ���������� ����������� ����� CRC32 �����
uint32_t calculateFileHash(const fs::path & path, uint64 fileSize, ProgressBar * progressBar) {
	
	/**
	 * ���� ���� ������, ��� ����������� ����� ��������� ������ ����
	 * ���� ���� �� ������� �������, ��� ����������� ����� ��������� ������ 1
	 */
	
	/*
	 * ����� read ������ ifstream ������ ����� ������������ ���������� (� ������������� ���������),
	 * � ����� � ��� ���� ������������ ����������� ������ � ������� �� ����� C
	 */
	
	boost::crc_32_type hash;
	
	if (fileSize > 0) {
		FILE * f;
		_wfopen_s(&f, path.wstring().c_str(), L"rb");
		
		std::byte buf[BYTES_TO_READ + 1]; // ��������� ������ ��� '\0'
		
		if (f == nullptr) {
			HashError err;
			err.Set(string("open file err: " + path.string()).c_str(), "\n", yellow);
			throw (err);
			
		} else {
			for (int i = 0, fullBlocks = (int) fileSize / BYTES_TO_READ; i < fullBlocks; ++i) {
				fread(buf, BYTES_TO_READ, 1, f);
				hash.process_bytes(buf, BYTES_TO_READ);
				
				if (progressBar) { progressBar->Update(BYTES_TO_READ); }
			}
			
			int lastBlockSize = int(fileSize % BYTES_TO_READ);
			if (lastBlockSize != 0) { // ��������� ���������� ����� (���� �� ������ BYTES_TO_READ ������)
				fread(buf, BYTES_TO_READ, 1, f);
				hash.process_bytes(buf, lastBlockSize);
				
				if (progressBar) { progressBar->Update(lastBlockSize); }
			}
			
			fclose(f);
		}
	}
	
	return (uint32_t) hash.checksum();
}