#include "../include/engine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// =================== File Handling ===================

bool loadFileList(const string &listFilename, string *&filePaths, int &fileCount)
{
    ifstream fin(listFilename);
    if (!fin)
        return false;

    fileCount = 0;
    string temp;
    while (getline(fin, temp))
        if (!temp.empty())
            fileCount++;

    if (fileCount == 0)
        return false;

    fin.clear();
    fin.seekg(0);

    filePaths = new string[fileCount];
    int idx = 0;
    while (getline(fin, temp))
        if (!temp.empty())
            filePaths[idx++] = temp;

    fin.close();
    return true;
}

bool fileExists(const string &filename)
{
    ifstream fin(filename);
    return fin.good();
}

int countLines(const string &filename)
{
    ifstream fin(filename);
    if (!fin)
        return 0;
    int count = 0;
    string line;
    while (getline(fin, line))
        count++;
    return count;
}

bool loadSingleFile(const string &filename, string *&dataLines, int &lineCount)
{
    ifstream fin(filename);
    if (!fin)
        return false;

    int linesInFile = countLines(filename);
    string line;
    string *newArr = new string[lineCount + linesInFile];

    for (int i = 0; i < lineCount; i++)
        newArr[i] = dataLines[i];

    int idx = lineCount;
    while (getline(fin, line))
        newArr[idx++] = line;

    delete[] dataLines;
    dataLines = newArr;
    lineCount += linesInFile;
    return true;
}

bool loadDataFiles(string *filePaths, int fileCount, string *&dataLines, int &lineCount)
{
    dataLines = nullptr;
    lineCount = 0;

    string folder = "data/";

    for (int i = 0; i < fileCount; i++)
    {
        string fullPath = folder + filePaths[i];
        if (!fileExists(fullPath))
        {
            cout << "Warning: File not found → " << fullPath << endl;
            continue;
        }

        if (!loadSingleFile(fullPath, dataLines, lineCount))
            return false;
    }
    return true;
}

void showFileSummary(string *filePaths, int fileCount, int lineCount)
{
    cout << "\nLoaded " << fileCount << " files with " << lineCount << " total lines.\n";
}

void clearData(string *&dataLines, int &lineCount)
{
    if (dataLines)
    {
        delete[] dataLines;
        dataLines = nullptr;
        lineCount = 0;
    }
}

// =================== Search History ===================

// Read history from file, format: keyword|frequency
void readSearchHistory(string *&history, int *&frequency, int &historyCount)
{
    ifstream fin("history.txt");
    if (!fin)
    {
        history = nullptr;
        frequency = nullptr;
        historyCount = 0;
        return;
    }

    // Count valid lines first
    historyCount = 0;
    string line;
    while (getline(fin, line))
        if (!line.empty())
            historyCount++;

    if (historyCount == 0)
    {
        history = nullptr;
        frequency = nullptr;
        return;
    }

    history = new string[historyCount];
    frequency = new int[historyCount];

    fin.clear();
    fin.seekg(0);

    int idx = 0;
    while (getline(fin, line))
    {
        if (line.empty()) continue;
        size_t sep = line.find('|');
        if (sep != string::npos)
        {
            history[idx] = line.substr(0, sep);
            frequency[idx] = stoi(line.substr(sep + 1));
            idx++;
        }
    }
}

// Save/Update single keyword to history file
void logSearchToFile(const string &keyword, string *&history, int *&frequency, int &historyCount)
{
    bool found = false;
    for (int i = 0; i < historyCount; i++)
    {
        if (history[i] == keyword)
        {
            frequency[i]++;
            found = true;
            break;
        }
    }

    if (!found)
    {
        // Add new keyword
        string *newHist = new string[historyCount + 1];
        int *newFreq = new int[historyCount + 1];

        for (int i = 0; i < historyCount; i++)
        {
            newHist[i] = history[i];
            newFreq[i] = frequency[i];
        }

        newHist[historyCount] = keyword;
        newFreq[historyCount] = 1;

        delete[] history;
        delete[] frequency;
        history = newHist;
        frequency = newFreq;
        historyCount++;
    }

    // Rewrite the entire file
    ofstream fout("history.txt", ios::trunc);
    if (fout.is_open())
    {
        for (int i = 0; i < historyCount; i++)
            fout << history[i] << "|" << frequency[i] << "\n";
        fout.close();
    }
}
