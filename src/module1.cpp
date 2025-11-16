#include "../include/engine.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool loadFileList(const string &listFilename, string *&filePaths, int &fileCount){
    ifstream fin(listFilename);
    if(!fin) return false;

    fileCount = 0;
    string temp;
    while(getline(fin,temp)) if(!temp.empty()) fileCount++;
    if(fileCount==0) return false;

    fin.clear();
    fin.seekg(0);
    filePaths = new string[fileCount];
    int idx=0;
    while(getline(fin,temp)) if(!temp.empty()) filePaths[idx++] = temp;

    fin.close();
    return true;
}

bool fileExists(const string &filename){
    ifstream fin(filename);
    return fin.good();
}

int countLines(const string &filename){
    ifstream fin(filename);
    if(!fin) return 0;
    int count=0;
    string line;
    while(getline(fin,line)) count++;
    return count;
}

bool loadSingleFile(const string &filename, string *&dataLines, int &lineCount){
    ifstream fin(filename);
    if(!fin) return false;

    int linesInFile = countLines(filename);
    string *newArr = new string[lineCount + linesInFile];
    for(int i=0;i<lineCount;i++) newArr[i] = dataLines[i];

    int idx=lineCount;
    string line;
    while(getline(fin,line)) newArr[idx++] = line;

    delete[] dataLines;
    dataLines = newArr;
    lineCount += linesInFile;
    return true;
}

bool loadDataFiles(string *filePaths, int fileCount, string *&dataLines, int &lineCount){
    dataLines = nullptr;
    lineCount = 0;
    string folder = "data/";

    for(int i=0;i<fileCount;i++){
        string fullPath = folder + filePaths[i];
        if(!fileExists(fullPath)){
            cout << "Warning: File not found → " << fullPath << endl;
            continue;
        }
        if(!loadSingleFile(fullPath, dataLines, lineCount)) return false;
    }
    return true;
}

void showFileSummary(string *filePaths, int fileCount, int lineCount){
    cout << "\nLoaded " << fileCount << " files with " << lineCount << " total lines.\n";
}

void clearData(string *&dataLines, int &lineCount){
    if(dataLines){ delete[] dataLines; dataLines=nullptr; lineCount=0; }
}

// ---------------------
// History handling (vectorless)
// ---------------------
void readSearchHistory(string *&history, int &historyCount){
    ifstream fin("history.txt");
    if(!fin){ history=nullptr; historyCount=0; return; }

    // First, count non-empty lines
    historyCount = 0;
    string line;
    while(getline(fin,line)) if(!line.empty()) historyCount++;

    if(historyCount == 0){ history = nullptr; return; }

    // Allocate array and store lines
    history = new string[historyCount];
    fin.clear();
    fin.seekg(0);
    int idx = 0;
    while(getline(fin,line)){
        if(!line.empty()) history[idx++] = line;
    }
}

void logSearchToFile(const string &keyword){
    if(keyword.empty()) return;
    ofstream fout("history.txt", ios::app);
    if(fout){ fout << keyword << endl; fout.close(); }
}
