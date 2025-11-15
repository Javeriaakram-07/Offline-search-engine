#ifndef ENGINE_H
#define ENGINE_H

#include <string>
using namespace std;

/* ============================================================
   MODULE 1 — DATA LOADING & FILE HANDLING
   ============================================================ */

// Reads file names from filelist.txt into a dynamic array
bool loadFileList(const string& listFilename, string*& filePaths, int& fileCount);

// Checks if a file exists
bool fileExists(const string& filename);

// Counts total lines inside a text file
int countLines(const string& filename);

// Loads a single text file into a dynamic array (appends to master array)
bool loadSingleFile(const string& filename, string*& dataLines, int& lineCount);

// Loads all files listed in filePaths into dataLines
bool loadDataFiles(string* filePaths, int fileCount, string*& dataLines, int& lineCount);

// Saves all loaded text data into binary format
bool saveToBinary(const string& binaryFilename, string* dataLines, int lineCount);

// Reads binary file back into dynamic string array
bool readFromBinary(const string& binaryFilename, string*& dataLines, int& lineCount);

// Shows summary of how many files and lines were loaded
void showFileSummary(string* filePaths, int fileCount, int lineCount);

// Frees dynamically allocated text data
void clearData(string*& dataLines, int& lineCount);


/* ============================================================
   MODULE 2 — SEARCH ENGINE CORE
   ============================================================ */

// Converts string to lowercase
string toLowerCase(const string& s);

// Exact keyword match
bool matchExact(const string& line, const string& keyword);

// Partial keyword match (keyword inside larger word)
bool matchPartial(const string& line, const string& keyword);

// Performs exact search
void simpleSearch(string* dataLines, int lineCount,
                  const string& keyword,
                  string*& results, int& resultCount);

// Performs partial search (fallback)
void partialMatchSearch(string* dataLines, int lineCount,
                        const string& keyword,
                        string*& results, int& resultCount);

// Handles “no result found” logic
bool handleNoResults(int resultCount, const string& keyword);

// Logs keyword + hits to history file
void logSearch(const string& keyword, int hits);

// Loads history file into dynamic array
void readSearchHistory(string*& history, int& historyCount);


/* ============================================================
   MODULE 3 — USER INTERFACE / MENUS
   ============================================================ */

// Displays welcome header
void showWelcome();

// Shows main menu and returns user choice
int displayMainMenu();

// Handles complete search process
void searchMenu(string* dataLines, int lineCount);

// Displays results with pagination
void displayResults(string* results, int resultCount);

// Shows special message when no results found
void displayNoResultMessage(const string& keyword);

// Displays search history
void displayHistory(string* history, int historyCount);

// Waits for user to press enter
void waitForEnter();

// Clears screen (platform dependent)
void clearScreen();

#endif