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
	/// ��� ������� �������� ������� (����, �����)
	enum FilesystemObjectType { file, directory };
	
	/// ������ �������� �������
	struct FilesystemObject {
		fs::path path; ///< ���� � �������
		uint64_t size = 0; ///< ������ ������� � ������ (������ ��� ������)
		uint32_t hash = 0; ///< ��� �������
		FilesystemObjectType type; ///< ��� �������
		
		list<FilesystemObject> files; ///< ��������� ����� (������ ��� ����������)
		list<FilesystemObject> dirs; ///< ��������� ����� (������ ��� ����������)
		
		FilesystemObject() = default;
		FilesystemObject(const fs::path & path, FilesystemObjectType type, uint64 size);
	};
	
	explicit DirectoryTree(const fs::path & root);
	
	/// ����� ����������� ������� ������ ����������
	void Walk();
	
	/// ����� ����� ������
	void DisplayRoot();
	
	/// ����� ����������� ������� ������ ����������
	void DisplayTree();
	
	/// ����� ������� ���������� ���� ��������� ��������
	void CalculateRootHash(int amountOfThreads);
	
	/// ���������� ������ ������
	const FilesystemObject & GetRoot();
	
	/// ���������� ���������� ��������� ������
	uint32_t GetAmountOfFiles() const;
	
	/// ���������� ���������� ��������� ����������
	uint32_t GetAmountOfDirs() const;
	
	/// ���������� ������ ����� ������
	uint64_t GetRootSize();
	
	/// ���������� ������ ������ �������� ���������� � �� ��������� ���������
	list<FilesystemObject *> GetFiles();

private:
	struct DirectoryTreeError : public Error {};
	
	FilesystemObject root; ///< ������ ������
	
	uint32_t amountOfFiles = 0; ///< ���������� ��������� ������
	uint32_t amountOfDirs = 0; ///< ���������� ��������� ����������
	
	/// ����������� ����� ����������
	void walk(const fs::path & path, FilesystemObject & currentObj);
	
	/// ��������� ������ �������
	static void displayIndent(int level, const string & indent);
	
	/// ����� �������� �����
	static void displayFile(const FilesystemObject & file, int indent);
	
	/// ����� �������� ����������
	static void displayDirectory(const FilesystemObject & dir, int indent);
	
	/// ����������� ������� ������ ������ ����������
	void displayTree(const FilesystemObject & currentObj, int indentSize);
	
	/// ���������� ���� �������� �����
	void calculateDirectoryHash(FilesystemObject & currentObj/*, ProgressBar * progressBar = nullptr*/);
	
	/// ������������� ������� �������� ����� ������ ���������� � �� ��������� ���������
	void calculateDirectoryHash(FilesystemObject & dir,
	                            int amountOfThreads,
	                            ProgressBar * progressBar = nullptr,
	                            DelayedErrorsBuffer * errorsBuffer = nullptr);
	
	/// ������������� ������� �������� ����� ������ ���������� � �� ��������� ���������
	void calculateFilesHash(list<FilesystemObject *> & files,
	                        mutex & mtx,
	                        ProgressBar * progressBar = nullptr,
	                        DelayedErrorsBuffer * errorsBuffer = nullptr);
	
	/// ���������� ������� ���������� � ������
	static uint64 calculateDirectorySize(const FilesystemObject & dir);
	
	/// ���������� ������ ������ ���������� � ������
	static void directoryFiles(FilesystemObject & dir, list<FilesystemObject *> & files);
	
	/// ����������� ���������� ������ �������� ���������� � �� ��������� ��������� � ������
	void files(FilesystemObject & dir, list<FilesystemObject *> & dirFiles);
	
	/// ���������� ������ ������ ���������� � �� ��������� ���������
	list<FilesystemObject *> getDirectoryFiles(FilesystemObject & dir);
};

#endif //UTILITIES_DIRECTORYTREE_H
