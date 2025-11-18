#include "../include/engine.h"
#include <cctype>
#include <limits>
using namespace std;

#define MAX_KEYWORDS 10
#define MAX_RESULTS 1000

bool correctionMade = false; // Flag to track if any correction happened

string toLowerCase(const string &s)
{
    string r = "";
    for (size_t i = 0; i < s.length(); i++)
        r += tolower(s[i]);
    return r;
}

int levenshteinDistance(const string &s1, const string &s2)
{
    int len1 = s1.length();
    int len2 = s2.length();
    int dp[100][100]; // adjust size if needed

    for (int i = 0; i <= len1; i++)
        dp[i][0] = i;
    for (int j = 0; j <= len2; j++)
        dp[0][j] = j;

    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1]));
        }
    }
    return dp[len1][len2];
}

// Find the closest match in the dataset
string didYouMean(const string &input, string *dataset, int lineCount)
{
    const int MAX_WORDS = 15;
    string words[MAX_WORDS];
    int wordCount = splitWords(input, words, MAX_WORDS);

    string correctedQuery = "";
    correctionMade = false;

    for (int i = 0; i < wordCount; i++)
    {
        string closestWord = words[i]; // keep original by default
        int minDist = numeric_limits<int>::max();

        for (int j = 0; j < lineCount; j++)
        {
            string lineWords[MAX_WORDS];
            int lineWordCount = splitWords(dataset[j], lineWords, MAX_WORDS);

            for (int k = 0; k < lineWordCount; k++)
            {
                if (lineWords[k].empty())
                    continue;

                int dist = levenshteinDistance(toLowerCase(words[i]), toLowerCase(lineWords[k]));
                if (dist < minDist)
                {
                    minDist = dist;
                    closestWord = lineWords[k];
                }
            }
        }

        // Replace only if reasonably close
        if (minDist > 0 && minDist <= 3 && closestWord != words[i])
        {
            correctionMade = true; // correction happened
            correctedQuery += closestWord;
        }
        else
        {
            correctedQuery += words[i]; // keep original word
        }

        if (i != wordCount - 1)
            correctedQuery += " ";
    }

    if (!correctionMade)
        return ""; // nothing changed
    return correctedQuery;
}

int splitWords(const string &line, string *words, int maxWords)
{
    int count = 0;
    string w = "";
    for (size_t i = 0; i <= line.length() && count < maxWords; i++)
    {
        char c = (i < line.length()) ? line[i] : ' ';
        if (isspace(c))
        {
            if (!w.empty())
            {
                words[count++] = w;
                w = "";
            }
        }
        else
            w += c;
    }
    return count;
}

// Calculate match percentage using Levenshtein distance
int calculateMatchPercentage(const string &original, const string &suggestion)
{
    if (original.empty() || suggestion.empty())
        return 0;

    int dist = levenshteinDistance(toLowerCase(original), toLowerCase(suggestion));
    int maxLen = max(original.length(), suggestion.length());
    if (maxLen == 0)
        return 0;

    double similarity = (1.0 - (double)dist / maxLen) * 100.0;
    if (similarity < 0)
        similarity = 0;
    if (similarity > 100)
        similarity = 100;

    return (int)(similarity + 0.5);
}

bool matchExact(const string &line, const string &keyword)
{
    string l = toLowerCase(line), k = toLowerCase(keyword);
    int lLen = l.length(), kLen = k.length();
    for (int i = 0; i <= lLen - kLen; i++)
    {
        bool ok = true;
        for (int j = 0; j < kLen; j++)
            if (l[i + j] != k[j])
            {
                ok = false;
                break;
            }
        if (ok)
        {
            bool before = (i == 0 || !isalnum(l[i - 1]));
            bool after = (i + kLen >= lLen || !isalnum(l[i + kLen]));
            if (before && after)
                return true;
        }
    }
    return false;
}

bool matchPartial(const string &line, const string &keyword)
{
    if (keyword.length() < 3)
        return false;
    string k = toLowerCase(keyword);
    string words[100];
    int wCount = splitWords(line, words, 100);
    for (int i = 0; i < wCount; i++)
    {
        string w = toLowerCase(words[i]);
        int wLen = w.length(), kLen = k.length();
        for (int j = 0; j <= wLen - kLen; j++)
        {
            bool match = true;
            for (int m = 0; m < kLen; m++)
                if (w[j + m] != k[m])
                {
                    match = false;
                    break;
                }
            if (match)
                return true;
        }
    }
    return false;
}

int searchMultiple(const string *keywords, int keywordCount, string *dataset, int lineCount, string *results, int maxResults)
{
    int found = 0;
    for (int i = 0; i < lineCount && found < maxResults; i++)
    {
        bool all = true;
        for (int k = 0; k < keywordCount; k++)
            if (!(matchExact(dataset[i], keywords[k]) || matchPartial(dataset[i], keywords[k])))
            {
                all = false;
                break;
            }
        if (all)
            results[found++] = dataset[i];
    }

    return found;
}

int searchEngine(const string &inputLine, string *dataset, int lineCount, string *results, int maxResults)
{
    string keywords[MAX_KEYWORDS];
    int keywordCount = splitWords(inputLine, keywords, MAX_KEYWORDS);
    return searchMultiple(keywords, keywordCount, dataset, lineCount, results, maxResults);
}
