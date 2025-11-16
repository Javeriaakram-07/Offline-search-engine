#include <iostream>
#include <limits>
#include "../include/engine.h"
using namespace std;

int main() {
    string *filePaths = nullptr;
    int fileCount = 0;

    string *dataLines = nullptr;
    int lineCount = 0;

    // History now uses TWO arrays:
    string *searchHistory = nullptr;   // stores keywords
    int *frequency = nullptr;          // stores frequency for each keyword
    int historyCount = 0;

    showWelcome();

    cout << "Loading files..." << endl;
    if (loadFileList("data/filelist.txt", filePaths, fileCount)) {
        if (loadDataFiles(filePaths, fileCount, dataLines, lineCount)) {
            cout << "Success! Loaded " << fileCount << " files." << endl;
        } else {
            cout << "Failed to load file data." << endl;
        }
    } else {
        cout << "Failed to read file list." << endl;
    }

    // NEW HISTORY LOADER
    readSearchHistory(searchHistory, frequency, historyCount);

    waitForEnter();

    bool programRunning = true;
    while (programRunning) {
        showWelcome();
        int choice = displayMainMenu();

        switch (choice) {
            case 1:
                searchMenu(dataLines, lineCount, searchHistory, frequency, historyCount);
                break;

            case 2:
                displayHistory(searchHistory, frequency, historyCount, dataLines, lineCount);
                break;

            case 3:
                showFileSummary(filePaths, fileCount, lineCount);
                break;

            case 4:
                programRunning = false;
                break;

            default:
                cout << "Invalid choice!\n";
                waitForEnter();
                break;
        }
    }

    clearData(dataLines, lineCount);
    delete[] filePaths;
    delete[] searchHistory;
    delete[] frequency;

    clearScreen();
    cout << "==========================================\n";
    cout << "  Thank you for using the Search Engine!\n";
    cout << "              Goodbye!\n";
    cout << "==========================================\n";
}
