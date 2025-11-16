#include <iostream>
#include <limits>
#include "../include/engine.h"
using namespace std;

int main() {
    string *filePaths = nullptr;
    int fileCount = 0;

    string *dataLines = nullptr;
    int lineCount = 0;

    string *searchHistory = new string[1000]; // max 1000 history entries
    int historyCount = 0;

    showWelcome();

    cout << "Loading files..." << endl;
    if(loadFileList("data/filelist.txt", filePaths, fileCount)){
        if(loadDataFiles(filePaths, fileCount, dataLines, lineCount)){
            cout<<"Success! Loaded "<<fileCount<<" files."<<endl;
        } else cout<<"Failed to load file data."<<endl;
    } else cout<<"Failed to read file list."<<endl;

    readSearchHistory(searchHistory, historyCount);

    waitForEnter();

    bool programRunning = true;
    while(programRunning){
        showWelcome();
        int choice = displayMainMenu();

        switch(choice){
            case 1: searchMenu(dataLines, lineCount, searchHistory, historyCount); break;
            case 2: displayHistory(searchHistory, historyCount, dataLines, lineCount); break;
            case 3: showFileSummary(filePaths, fileCount, lineCount); break;
            case 4: programRunning=false; break;
            default: cout<<"Invalid choice!\n"; waitForEnter(); break;
        }
    }

    clearData(dataLines, lineCount);
    delete[] filePaths;
    delete[] searchHistory;

    clearScreen();
    cout<<"=========================================="<<endl;
    cout<<"  Thank you for using the Search Engine!"<<endl;
    cout<<"              Goodbye!"<<endl;
    cout<<"=========================================="<<endl;
}
