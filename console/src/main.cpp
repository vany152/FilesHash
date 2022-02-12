#include <iostream>
#include <filesystem>
#include <windows.h>

#include "common.h"
#include "Timer.h"
#include "DirectoryTree.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

using namespace std;
namespace fs = std::filesystem;

int main() {
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, ".1251");
	
	while (true) {
		try {
			DirectoryTree tree(inputPath());
			int amountOfThreads = inputAmountOfThreads();
			tree.Walk();
			
			cout << '\n';
			if (tree.GetRoot().type == DirectoryTree::directory) {
				cout << "папок: " << tree.GetAmountOfDirs() << ";\tфайлов: " << tree.GetAmountOfFiles() << "\n";
			}
			cout << "размер: ";
			displaySize(tree.GetRootSize());
			cout << "\n";
			
			Timer timer;
			
			timer.Start();
			tree.CalculateRootHash(amountOfThreads);
			timer.Stop();
			
			cout << endl;
			tree.DisplayRoot();
			cout << timer.Duration() << '\n';
			
			if (tree.GetRoot().type == DirectoryTree::directory) {
				cout << "вывести дерево каталога?\n";
				string choice = inputChoice();
				if (choice == "y") { tree.DisplayTree(); }
			}
			
		} catch (std::filesystem::filesystem_error & err) {
			displayError(err.what());
		}
		
		cout << "\n====================================================\n\n";
	}
}

#pragma clang diagnostic pop