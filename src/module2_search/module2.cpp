#include "../../include/engine.h"
#include <fstream>
#include <cctype>
using namespace std;

#define MAX_KEYWORDS 10
#define MAX_RESULTS 1000

// Convert string to lowercase
string toLowerCase(const string &s) {
    string result = s;
    for (size_t i = 0; i < result.length(); i++)
        result[i] = tolower(result[i]);
    return result;
}

// Split a line into words (manual parsing)
int splitWords(const string &line, string *words, int maxWords) {
    int count = 0;
    string current = "";
    for (size_t i = 0; i <= line.length() && count < maxWords; i++) {
        char c = (i < line.length()) ? line[i] : ' ';
        if (isspace(c)) {
            if (!current.empty()) {
                words[count++] = current;
                current = "";
            }
        } else {
            current += c;
        }
    }
    return count;
}

// Exact match (whole word)
bool matchExact(const string &line, const string &keyword) {
    string lowerLine = toLowerCase(line);
    string lowerKeyword = toLowerCase(keyword);
    int lineLen = lowerLine.length();
    int keyLen = lowerKeyword.length();

    for (int i = 0; i <= lineLen - keyLen; i++) {
        bool match = true;
        for (int j = 0; j < keyLen; j++) {
            if (lowerLine[i + j] != lowerKeyword[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            bool beforeOK = (i == 0 || !isalnum(lowerLine[i - 1]));
            bool afterOK = (i + keyLen >= lineLen || !isalnum(lowerLine[i + keyLen]));
            if (beforeOK && afterOK) return true;
        }
    }
    return false;
}

// Partial match (keyword inside a word, length >= 3)
bool matchPartial(const string &line, const string &keyword) {
    if (keyword.length() < 3) return false;
    string lowerKeyword = toLowerCase(keyword);
    string words[100];
    int wordCount = splitWords(line, words, 100);
    for (int i = 0; i < wordCount; i++) {
        string w = toLowerCase(words[i]);
        int wLen = w.length();
        int kLen = lowerKeyword.length();
        for (int j = 0; j <= wLen - kLen; j++) {
            bool match = true;
            for (int k = 0; k < kLen; k++) {
                if (w[j + k] != lowerKeyword[k]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
    }
    return false;
}

// Search multiple keywords in dataset
int searchMultiple(const string *keywords, int keywordCount, string *dataset, int lineCount, string *results, int maxResults) {
    int found = 0;

    // First: lines containing ALL keywords
    for (int i = 0; i < lineCount && found < maxResults; i++) {
        bool allKeywordsInLine = true;
        for (int k = 0; k < keywordCount; k++) {
            if (!(matchExact(dataset[i], keywords[k]) || matchPartial(dataset[i], keywords[k]))) {
                allKeywordsInLine = false;
                break;
            }
        }
        if (allKeywordsInLine) results[found++] = dataset[i];
    }

    // Second: lines containing ANY keyword (if none had all)
    if (found == 0) {
        for (int i = 0; i < lineCount && found < maxResults; i++) {
            for (int k = 0; k < keywordCount; k++) {
                if (matchExact(dataset[i], keywords[k]) || matchPartial(dataset[i], keywords[k])) {
                    results[found++] = dataset[i];
                    break;
                }
            }
        }
    }

    return found;
}

// Public function called by UI
int searchEngine(const string &inputLine, string *dataset, int lineCount, string *results, int maxResults) {
    string keywords[MAX_KEYWORDS];
    int keywordCount = splitWords(inputLine, keywords, MAX_KEYWORDS);
    return searchMultiple(keywords, keywordCount, dataset, lineCount, results, maxResults);
}
