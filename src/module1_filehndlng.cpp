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


// =================== Search History (Binary Version) ===================

void readSearchHistory(string *&history, int *&frequency, int &historyCount)
{
    ifstream fin("history.bin", ios::binary);
    if (!fin)
    {
        history = nullptr;
        frequency = nullptr;
        historyCount = 0;
        return;
    }

    // First, read count
    fin.read(reinterpret_cast<char *>(&historyCount), sizeof(int));
    if (historyCount <= 0)
    {
        history = nullptr;
        frequency = nullptr;
        historyCount = 0;
        return;
    }

    history = new string[historyCount];
    frequency = new int[historyCount];

    for (int i = 0; i < historyCount; i++)
    {
        int len;
        fin.read(reinterpret_cast<char *>(&len), sizeof(int));

        char *buffer = new char[len + 1];
        fin.read(buffer, len);
        buffer[len] = '\0';
        history[i] = buffer;
        delete[] buffer;

        fin.read(reinterpret_cast<char *>(&frequency[i]), sizeof(int));
    }

    fin.close();
}

// Save/Update single keyword to binary file
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

    // Now write FULL updated history to binary
    ofstream fout("history.bin", ios::binary | ios::trunc);
    if (fout.is_open())
    {
        // Save total count first
        fout.write(reinterpret_cast<char *>(&historyCount), sizeof(int));

        for (int i = 0; i < historyCount; i++)
        {
            int len = history[i].size();
            fout.write(reinterpret_cast<char *>(&len), sizeof(int));
            fout.write(history[i].c_str(), len);
            fout.write(reinterpret_cast<char *>(&frequency[i]), sizeof(int));
        }

        fout.close();
    }
}
