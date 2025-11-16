#include"../../include/engine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

/* ============================================================
   MODULE 1 — DATA LOADING & FILE HANDLING IMPLEMENTATION
   ============================================================ */

/* Reads all file names from filelist.txt into dynamic array */
bool loadFileList(const string& listFilename, string*& filePaths, int& fileCount)
{
    ifstream fin(listFilename);
    if (!fin)
        return false;

    fileCount = 0;
    string temp;

    // First count how many files are listed
    while (getline(fin, temp))
        if (!temp.empty()) fileCount++;

    if (fileCount == 0)
        return false;

    fin.clear();
    fin.seekg(0);

    filePaths = new string[fileCount];
    int index = 0;

    // Load filenames
    while (getline(fin, temp))
    {
        if (!temp.empty())
            filePaths[index++] = temp;
    }

    fin.close();
    return true;
}

/* Checks if a file exists */
bool fileExists(const string& filename)
{
    ifstream fin(filename);
    return fin.good();
}

/* Count total lines inside a text file */
int countLines(const string& filename)
{
    ifstream fin(filename);
    if (!fin) return 0;

    int count = 0;
    string line;
    while (getline(fin, line))
        count++;

    fin.close();
    return count;
}

/* Loads a single text file into dynamic array (appends) */
bool loadSingleFile(const string& filename, string*& dataLines, int& lineCount)
{
    ifstream fin(filename);
    if (!fin) return false;

    string line;
    int linesInFile = countLines(filename);

    if (linesInFile == 0)
        return true;

    // Create new larger array
    string* newArr = new string[lineCount + linesInFile];

    // Copy old data
    for (int i = 0; i < lineCount; i++)
        newArr[i] = dataLines[i];

    // Append new file data
    int idx = lineCount;
    while (getline(fin, line))
        newArr[idx++] = line;

    // Delete old array
    delete[] dataLines;

    // Replace with new array
    dataLines = newArr;
    lineCount += linesInFile;

    fin.close();
    return true;
}

/* Loads all files listed in filePaths into dataLines */
bool loadDataFiles(string* filePaths, int fileCount, string*& dataLines, int& lineCount)
{
    dataLines = nullptr;
    lineCount = 0;

    for (int i = 0; i < fileCount; i++)
    {
        if (!fileExists(filePaths[i]))
        {
            cout << "Warning: File not found → " << filePaths[i] << endl;
            continue;
        }

        if (!loadSingleFile(filePaths[i], dataLines, lineCount))
            return false;
    }

    return true;
}

/* Saves all loaded text data into binary format */
bool saveToBinary(const string& binaryFilename, string* dataLines, int lineCount)
{
    ofstream fout(binaryFilename, ios::binary);
    if (!fout) return false;

    fout.write((char*)&lineCount, sizeof(lineCount));

    for (int i = 0; i < lineCount; i++)
    {
        int len = dataLines[i].size();
        fout.write((char*)&len, sizeof(len));
        fout.write(dataLines[i].c_str(), len);
    }

    fout.close();
    return true;
}

/* Reads binary file back into dynamic string array */
bool readFromBinary(const string& binaryFilename, string*& dataLines, int& lineCount)
{
    ifstream fin(binaryFilename, ios::binary);
    if (!fin) return false;

    fin.read((char*)&lineCount, sizeof(lineCount));

    dataLines = new string[lineCount];

    for (int i = 0; i < lineCount; i++)
    {
        int len;
        fin.read((char*)&len, sizeof(len));

        char* buffer = new char[len + 1];
        fin.read(buffer, len);
        buffer[len] = '\0';

        dataLines[i] = buffer;
        delete[] buffer;
    }

    fin.close();
    return true;
}

/* Shows summary of loaded files */
void showFileSummary(string* filePaths, int fileCount, int lineCount)
{
    cout << "\n==================================================\n";
    cout << " FILE SUMMARY\n";
    cout << "==================================================\n";

    cout << "Total files loaded : " << fileCount << endl;
    for (int i = 0; i < fileCount; i++)
        cout << "  - " << filePaths[i] << endl;

    cout << "Total text lines  : " << lineCount << endl;
    cout << "==================================================\n";
}

/* Safely frees allocated text data */
void clearData(string*& dataLines, int& lineCount)
{
    if (dataLines)
    {
        delete[] dataLines;
        dataLines = nullptr;
        lineCount = 0;
    }
}

/* Load search history into dynamic array */
void readSearchHistory(string*& history, int& historyCount)
{
    ifstream fin("history.txt");
    if (!fin.is_open())
    {
        history = nullptr;
        historyCount = 0;
        return;
    }

    // Count lines
    historyCount = 0;
    string temp;
    while (getline(fin, temp))
        historyCount++;

    fin.close();

    if (historyCount == 0)
    {
        history = nullptr;
        return;
    }

    fin.open("history.txt");
    history = new string[historyCount];

    int i = 0;
    while (getline(fin, temp))
        history[i++] = temp;

    fin.close();
}