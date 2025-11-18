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
void searchMenu(string *dataLines, int lineCount, string *&searchHistory, int *&frequency, int &historyCount);
void displayHistory(string *&history, int *&frequency, int &historyCount, string *dataLines, int lineCount);



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
void readSearchHistory(string *&history, int *&frequency, int &historyCount);
void logSearchToFile(const string &keyword, string *&history, int *&frequency, int &historyCount);

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
int calculateMatchPercentage(const string &original, const string &suggestion);






#endif
