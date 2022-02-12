#ifndef UTILITIES_DIRECTORYTREE_H
#define UTILITIES_DIRECTORYTREE_H

#define OPEN_FILE_ERROR 0x1

#include <iostream>
#include <filesystem>
#include <windows.h>
#include <list>
#include <thread>
#include <mutex>
#include <boost/crc.hpp>

#include "common.h"
#include "hash.h"
#include "ProgressBar.h"
#include "DelayedErrorsBuffer.h"

using namespace std;
namespace fs = filesystem;

class DirectoryTree {
public:
	/// тип объекта файловой системы (файл, папка)
	enum FilesystemObjectType { file, directory };
	
	/// объект файловой системы
	struct FilesystemObject {
		fs::path path; ///< путь к объекту
		uint64_t size = 0; ///< размер объекта в байтах (только для файлов)
		uint32_t hash = 0; ///< хеш объекта
		FilesystemObjectType type; ///< тип объекта
		
		list<FilesystemObject> files; ///< вложенные файлы (только для директорий)
		list<FilesystemObject> dirs; ///< вложенные папки (только для директорий)
		
		FilesystemObject() = default;
		FilesystemObject(const fs::path & path, FilesystemObjectType type, uint64 size);
	};
	
	explicit DirectoryTree(const fs::path & root);
	
	/// вызов рекурсивной функции обхода директории
	void Walk();
	
	/// вывод корня дерева
	void DisplayRoot();
	
	/// вызов рекурсивной функции вывода директории
	void DisplayTree();
	
	/// вызов функции вычисления хеша корневого элемента
	void CalculateRootHash(int amountOfThreads);
	
	/// возвращает корень дерева
	const FilesystemObject & GetRoot();
	
	/// возвращает количество вложенных файлов
	uint32_t GetAmountOfFiles() const;
	
	/// возвращает количество вложенных директорий
	uint32_t GetAmountOfDirs() const;
	
	/// возвращает размер корня дерева
	uint64_t GetRootSize();
	
	/// возвращает список файлов корневой директории и ее вложенных каталогов
	list<FilesystemObject *> GetFiles();

private:
	struct DirectoryTreeError : public Error {};
	
	FilesystemObject root; ///< корень дерева
	
	uint32_t amountOfFiles = 0; ///< количество вложенных файлов
	uint32_t amountOfDirs = 0; ///< количество вложенных директорий
	
	/// рекурсивный обход директории
	void walk(const fs::path & path, FilesystemObject & currentObj);
	
	/// установка уровня отступа
	static void displayIndent(int level, const string & indent);
	
	/// вывод названия файла
	static void displayFile(const FilesystemObject & file, int indent);
	
	/// вывод названия директории
	static void displayDirectory(const FilesystemObject & dir, int indent);
	
	/// рекурсивная функция вывода дерева директории
	void displayTree(const FilesystemObject & currentObj, int indentSize);
	
	/// вычисление хеша корневой папки
	void calculateDirectoryHash(FilesystemObject & currentObj/*, ProgressBar * progressBar = nullptr*/);
	
	/// многопоточная функция подсчета хешей файлов директории и ее вложенных каталогов
	void calculateDirectoryHash(FilesystemObject & dir,
	                            int amountOfThreads,
	                            ProgressBar * progressBar = nullptr,
	                            DelayedErrorsBuffer * errorsBuffer = nullptr);
	
	/// многопоточная функция подсчета хешей файлов директории и ее вложенных каталогов
	void calculateFilesHash(list<FilesystemObject *> & files,
	                        mutex & mtx,
	                        ProgressBar * progressBar = nullptr,
	                        DelayedErrorsBuffer * errorsBuffer = nullptr);
	
	/// вычисление размера директории в байтах
	static uint64 calculateDirectorySize(const FilesystemObject & dir);
	
	/// добавление список файлов директории в список
	static void directoryFiles(FilesystemObject & dir, list<FilesystemObject *> & files);
	
	/// рекурсивное добавление файлов корневой директории и ее вложенных каталогов в список
	void files(FilesystemObject & dir, list<FilesystemObject *> & dirFiles);
	
	/// возвращает список файлов директории и ее вложенных каталогов
	list<FilesystemObject *> getDirectoryFiles(FilesystemObject & dir);
};

#endif //UTILITIES_DIRECTORYTREE_H
