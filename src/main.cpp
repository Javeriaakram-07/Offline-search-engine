#include <iostream>
#include <limits>
#include "../include/engine.h"
using namespace std;

int main()
{
    string *filePaths = nullptr;
    int fileCount = 0;

    string *dataLines = nullptr;
    int lineCount = 0;

    // History uses TWO arrays:
    string *searchHistory = nullptr; // stores keywords
    int *frequency = nullptr;        // stores frequency for each keyword
    int historyCount = 0;

    cout << "Loading files..." << endl;
    if (loadFileList("data/filelist.txt", filePaths, fileCount))
    {
        if (loadDataFiles(filePaths, fileCount, dataLines, lineCount))
        {
            cout << GREEN << "Success!" << RESET
                 << " Loaded " << BOLD << YELLOW << fileCount << RESET
                 << " files with total " << BOLD << CYAN << lineCount << RESET << " lines.\n";
        }
        else
        {
            cout << BRED << "Failed to load file data." << RESET << endl;
        }
    }
    else
    {
        cout << BRED << "Failed to read file list." << RESET << endl;
    }

    //  HISTORY LOADER
    readSearchHistory(searchHistory, frequency, historyCount);

    waitForEnter();

    bool programRunning = true;
    while (programRunning)
    {
        showWelcome();
        int choice = displayMainMenu();

        switch (choice)
        {
        case 1:
            searchMenu(dataLines, lineCount, searchHistory, frequency, historyCount);
            break;

        case 2:
            displayHistory(searchHistory, frequency, historyCount, dataLines, lineCount);
            break;

        case 3:
            showFileSummary(filePaths, fileCount, lineCount);
            waitForEnter();
            break;

        case 4:
            programRunning = false;
            break;

        default:
            cout << BRED << "Invalid choice!\n"
                 << RESET;
            cout << BMAGENTA << "Press ENTER to continue..." << RESET;
            waitForEnter();
            break;
        }
    }

    clearData(dataLines, lineCount);
    delete[] filePaths;
    delete[] searchHistory;
    delete[] frequency;

    clearScreen();
    cout << BOLD << PINK;
    cout << "==========================================\n";
    cout << "  Thank you for using the Search Engine!\n";
    cout << "              Goodbye!\n";
    cout << "==========================================\n";
    cout << RESET;
}
