#include "DirectoryTree.h"

/** *********************************************** PUBLIC SECTION ************************************************ **/

DirectoryTree::FilesystemObject::FilesystemObject(const fs::path & path, FilesystemObjectType type, uint64 size) {
	this->path = path;
	this->type = type;
	this->size = size;
}

DirectoryTree::DirectoryTree(const fs::path & root) {
	this->root = FilesystemObject(
			root,
			is_directory(root) ? directory : file,
			is_directory(root) ? 0 : file_size(root)
	);
}

/// ����� ����������� ������� ������ ����������
void DirectoryTree::Walk() {
	if (root.type == directory) { walk(root.path, root); }
}

/// ����� ����� ������
void DirectoryTree::DisplayRoot() {
	if (root.type == file) { displayFile(root, 0); }
	else { displayDirectory(root, 0); }
}

/// ����� ����������� ������� ������ ����������
void DirectoryTree::DisplayTree() {
	displayTree(root, 0);
}

/// ����� ������� ���������� ���� ��������� ��������
void DirectoryTree::CalculateRootHash(int amountOfThreads) {
	CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleBufferInfo);
	
	ProgressBar * progressBar = new ProgressBar(0,
	                                            root.size,
	                                            consoleBufferInfo.dwCursorPosition.X,
	                                            consoleBufferInfo.dwCursorPosition.Y);

	DelayedErrorsBuffer * errorsBuffer = new DelayedErrorsBuffer();
	
	if (root.type == directory) { calculateDirectoryHash(root, amountOfThreads, progressBar, errorsBuffer); }
	else { root.hash = calculateFileHash(root.path, root.size, progressBar); }
	
	delete progressBar;
	delete errorsBuffer;
}

/// ���������� ������ ������
const DirectoryTree::FilesystemObject & DirectoryTree::GetRoot() {
	return root;
}

/// ���������� ���������� ��������� ������
uint32_t DirectoryTree::GetAmountOfFiles() const {
	return amountOfFiles;
}

/// ���������� ���������� ��������� ����������
uint32_t DirectoryTree::GetAmountOfDirs() const {
	return amountOfDirs;
}

/// ���������� ������ ����� ������
uint64_t DirectoryTree::GetRootSize() {
	return root.size;
}

/// ���������� ������ ������ �������� ���������� � �� ��������� ���������
list<DirectoryTree::FilesystemObject *> DirectoryTree::GetFiles() {
	return getDirectoryFiles(root);
}

/** *********************************************** PRIVATE SECTION *********************************************** **/

/// ����������� ����� ����������
void DirectoryTree::walk(const fs::path & path, FilesystemObject & currentObj) {
	for (auto it = fs::directory_iterator(path); it != fs::directory_iterator(); ++it) {
		try {
			FilesystemObject obj(
					it->path(),
					it->is_directory() ? directory : file,
					it->is_directory() ? 0 : it->file_size()
			);
			
			if (obj.type == directory) {
				++amountOfDirs;
				walk(it->path(), obj);
				
				obj.size = calculateDirectorySize(obj);
				currentObj.dirs.push_back(obj);
				
			} else {
				++amountOfFiles;
				currentObj.files.push_back(obj);
				
			}
			
			currentObj.size += obj.size;
			
		} catch (fs::filesystem_error & err) {
			DirectoryTreeError error;
			error.Set(strcat((char *) "walk err: ", err.what()), "\n", yellow);
			error.What();
		}
	}
}

/// ��������� ������ �������
void DirectoryTree::displayIndent(int level, const string & indent = "      ") {
	for (int i = 0; i < level; ++i) { cout << indent; }
}

/// ����� �������� �����
void DirectoryTree::displayFile(const FilesystemObject & file, int indentSize) {
	displayIndent(indentSize);
	if (file.hash != 1) {
		changeConsoleTextColor(yellow);
		cout << hex << setfill('0') << setw(8) << right << file.hash << dec;
		changeConsoleTextColor(white);
		cout << "    " << file.path.filename() << endl;
		
	} else {
		changeConsoleTextColor(red);
		cout << file.path.filename() << "    " << "������ �������� �����, ��� �� �����������" << endl;
		changeConsoleTextColor(white);
	}
}

/// ����� �������� ����������
void DirectoryTree::displayDirectory(const DirectoryTree::FilesystemObject & dir, int indent) {
	displayIndent(indent);
	
	changeConsoleTextColor(cyan);
	cout << hex << dir.hash << dec << "    ";
	changeConsoleTextColor(white);
	
	changeConsoleTextColor(green);
	cout << dir.path.filename();
	changeConsoleTextColor(white);
	
	cout << endl;
}

/// ����������� ������� ������ ������ ����������
void DirectoryTree::displayTree(const DirectoryTree::FilesystemObject & currentObj, int indentSize) {
	displayDirectory(currentObj, indentSize);
	
	if (root.type == directory) {
		for (auto & file : currentObj.files) {
			displayFile(file, indentSize + 1);
		}
		
		for (auto & dir : currentObj.dirs) {
			cout << endl;
			displayTree(dir, indentSize + 1);
		}
		
	} else {
		displayFile(root, indentSize);
	}
}

/// ���������� ���� �������� �����
void DirectoryTree::calculateDirectoryHash(FilesystemObject & dir/*, ProgressBar * progressBar*/) {
	/**
	 * ����������� ����� ���������� ����������� ��� ��� ����������� ���� ��������� ������ � ���������
	 */
	
	for (auto & _dir : dir.dirs) {
		calculateDirectoryHash(_dir);
	}
	
	boost::crc_32_type hash;
	for (auto & file : dir.files) { // ���������� � ���� ����� ����� ��������� ������
		if (file.hash != 1) {
			hash.process_bytes(&file.hash, sizeof(dir.hash));
		}
	}
	
	for (auto & _dir : dir.dirs) { // ���������� � ���� ����� ����� ��������� �����
		hash.process_bytes(&dir.hash, sizeof(dir.hash));
	}
	
	dir.hash = hash.checksum();
}

/// ������������� ������� �������� ����� ������ ���������� � �� ��������� ���������
void DirectoryTree::calculateDirectoryHash(FilesystemObject & dir,
                                           int amountOfThreads,
                                           ProgressBar * progressBar,
                                           DelayedErrorsBuffer * errorsBuffer) {
	
	list<FilesystemObject *> files = getDirectoryFiles(dir);
	vector<thread> threads;
	mutex mtx;
	
	for (int i = 0; i < amountOfThreads; ++i) {
		threads.emplace_back(
				&DirectoryTree::calculateFilesHash,
				*this,
				ref(files),
				ref(mtx),
				progressBar,
				errorsBuffer
		);
	}
	
	for (auto & thr : threads) { thr.join(); }
	
	calculateDirectoryHash(dir);
}

/// ���������� ����� ������ �� ������
void DirectoryTree::calculateFilesHash(list<FilesystemObject *> & files,
                                       mutex & mtx,
                                       ProgressBar * progressBar,
                                       DelayedErrorsBuffer * errorsBuffer) {
	FilesystemObject * file;
	while (!files.empty()) {
		mtx.lock();
		file = files.front();
		files.erase(files.begin());
		mtx.unlock();
		
		try {
			file->hash = calculateFileHash(file->path, file->size);
			
		} catch (HashError & err) {
			file->hash = OPEN_FILE_ERROR;
			if (progressBar && errorsBuffer) { errorsBuffer->AddErrorToBuffer(err); }
			else { err.What(); }
		}
		
		if (progressBar) {
			mtx.lock();
			progressBar->Update(file->size);
			mtx.unlock();
		}
	}
}

/// ���������� ������� ���������� � ������
uint64 DirectoryTree::calculateDirectorySize(const FilesystemObject & dir) {
	uint64 dirSize = 0;
	
	for (auto & file : dir.files) {
		dirSize += file.size;
	}
	
	for (auto & _dir : dir.dirs) {
		dirSize += _dir.size;
	}
	
	return dirSize;
}

/// ���������� ������ ������ ���������� � ������
inline void DirectoryTree::directoryFiles(DirectoryTree::FilesystemObject & dir, list<FilesystemObject *> & files) {
	for (auto & file : dir.files) { files.push_back(&file); }
}

/// ����������� ���������� ������ �������� ���������� � �� ��������� ��������� � ������
void DirectoryTree::files(FilesystemObject & dir, list<FilesystemObject *> & dirFiles) {
	for (auto & _dir : dir.dirs) { files(_dir, dirFiles); }
	directoryFiles(dir, dirFiles);
}

/// ���������� ������ ������ ���������� � �� ��������� ���������
list<DirectoryTree::FilesystemObject *> DirectoryTree::getDirectoryFiles(FilesystemObject & dir) {
	list<FilesystemObject *> dirFiles;
	files(dir, dirFiles);
	
	return dirFiles;
}

