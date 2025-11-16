#ifndef ENGINE_H
#define ENGINE_H

#include <string>
using namespace std;

// -----------------------------
// Console / UI functions
// -----------------------------
void clearScreen();
void waitForEnter();
void showWelcome();
int displayMainMenu();
void searchMenu(string *dataLines, int lineCount, string *&searchHistory, int &historyCount);
void displayHistory(string *&history, int &historyCount, string *dataLines, int lineCount);
void displayResults(string *results, int count);
void displayNoResultMessage(const string &keyword);

// -----------------------------
// File Handling / Data functions
// -----------------------------
bool loadFileList(const string &listFilename, string *&filePaths, int &fileCount);
bool fileExists(const string &filename);
int countLines(const string &filename);
bool loadSingleFile(const string &filename, string *&dataLines, int &lineCount);
bool loadDataFiles(string *filePaths, int fileCount, string *&dataLines, int &lineCount);
void showFileSummary(string *filePaths, int fileCount, int lineCount);
void clearData(string *&dataLines, int &lineCount);
void readSearchHistory(string *&history, int &historyCount);
void logSearchToFile(const string &keyword);

// -----------------------------
// Search Engine functions
// -----------------------------
int searchEngine(const string &inputLine, string *dataset, int lineCount, string *results, int maxResults);
int searchMultiple(const string *keywords, int keywordCount, string *dataset, int lineCount, string *results, int maxResults);
bool matchExact(const string &line, const string &keyword);
bool matchPartial(const string &line, const string &keyword);
int splitWords(const string &line, string *words, int maxWords);
string toLowerCase(const string &s);
int levenshteinDistance(const string &s1, const string &s2);
string didYouMean(const string &input, string *dataset, int lineCount);





#endif
