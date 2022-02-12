#include "hash.h"

#define BYTES_TO_READ 32

/// вычисление контрольной суммы CRC32 файла
uint32_t calculateFileHash(const fs::path & path, uint64 fileSize, ProgressBar * progressBar) {
	
	/**
	 * если файл пустой, его контрольная сумма считается равной нулю
	 * если файл не удалось открыть, его контрольная сумма считается равной 1
	 */
	
	/*
	 * метод read класса ifstream читает файлы недостаточно эффективно (с недостаточной скоростью),
	 * в связи с чем были использованы инструменты работы с файлами из языка C
	 */
	
	boost::crc_32_type hash;
	
	if (fileSize > 0) {
		FILE * f;
		_wfopen_s(&f, path.wstring().c_str(), L"rb");
		
		std::byte buf[BYTES_TO_READ + 1]; // последний символ для '\0'
		
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
			if (lastBlockSize != 0) { // обработка последнего блока (если он меньше BYTES_TO_READ байтов)
				fread(buf, BYTES_TO_READ, 1, f);
				hash.process_bytes(buf, lastBlockSize);
				
				if (progressBar) { progressBar->Update(lastBlockSize); }
			}
			
			fclose(f);
		}
	}
	
	return (uint32_t) hash.checksum();
}