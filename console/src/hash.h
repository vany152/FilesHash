#ifndef UTILITIES_HASH_H
#define UTILITIES_HASH_H

#include <filesystem>
#include <boost/crc.hpp>

#include "common.h"
#include "ProgressBar.h"

using namespace std;
namespace fs = filesystem;

struct HashError : public Error {};

/// вычисление контрольной суммы CRC32 файла
uint32_t calculateFileHash(const fs::path & path, uint64 fileSize, ProgressBar * progressBar = nullptr);

#endif //UTILITIES_HASH_H
