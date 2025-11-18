#ifndef ENGINE_H
#define ENGINE_H
// ===== Console Colors =====
#define RESET "\033[0m"

// Basic Colors
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Bright (High Contrast) Colors
#define BRED "\033[91m"
#define BGREEN "\033[92m"
#define BYELLOW "\033[93m"
#define BBLUE "\033[94m"
#define BMAGENTA "\033[95m"
#define BCYAN "\033[96m"
#define BWHITE "\033[97m"

// Useful for Highlight Types
#define ORANGE "\033[38;5;208m" // nice orangish
#define PINK "\033[38;5;205m"   // smooth pink
#define GREY "\033[90m"         // dark grey
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

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
