#include "../include/engine.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

// =================== Console UI ===================

void clearScreen()
{
    system("cls");
}

void waitForEnter()
{
    cout << BMAGENTA << "Press ENTER to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showWelcome()
{
    cout << BOLD << CYAN << "==== OFFLINE SEARCH ENGINE ====" << RESET << "\n";
}

int displayMainMenu()
{
    int choice;
    cout << "\n"
         << BOLD << BBLUE << "========== MAIN MENU ==========" << RESET << "\n";
    cout << GREEN << "1. Search Text\n"
         << CYAN << "2. View Search History\n"
         << ORANGE << "3. View File Information\n"
         << RED << BOLD << "4. Exit Program\n"
         << RESET;
    cout << GREY << "Enter a number to continue: " << RESET;

    while (!(cin >> choice) || choice < 1 || choice > 4)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << BRED << "Invalid! Enter 1-4: " << RESET;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

// =================== Search Menu ===================

void searchMenu(string *dataLines, int lineCount, string *&history, int *&frequency, int &historyCount)
{
    if (!dataLines || lineCount == 0)
    {
        cout << RED << "No files loaded." << RESET << "\n";
        waitForEnter();
        return;
    }

    string keyword;
    cout << BMAGENTA << "\nEnter keyword: " << RESET;
    getline(cin, keyword);
    if (keyword.empty())
    {
        cout << RED << "Empty input not allowed." << RESET << "\n";
        waitForEnter();
        return;
    }

    string results[1000];
    int found = searchEngine(keyword, dataLines, lineCount, results, 1000);

    if (found > 0)
    {
        cout << "\nFound " << BYELLOW << found << RESET << " results:\n";
        for (int i = 0; i < found; i++)
            cout << GREEN << i + 1 << ". " << RESET << results[i] << "\n";

        logSearchToFile(keyword, history, frequency, historyCount);
        waitForEnter();
        return;
    }

    // DidYouMean Attempt
    string suggestion = didYouMean(keyword, dataLines, lineCount);
    int matchPercent = calculateMatchPercentage(keyword, suggestion);

    if (correctionMade && !suggestion.empty())
    {
        cout << "\n"
             << BYELLOW << "Did you mean "
             << RESET << "\"" << ORANGE << suggestion << RESET << "\" "
             << "(" << PINK << matchPercent << RESET << GREY << "% match)? "
             << CYAN << "(1 = Yes / 0 = No): " << RESET;

        int ans;
        while (!(cin >> ans) || (ans != 0 && ans != 1))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter 1 for Yes, 0 for No: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ans == 1)
        {
            keyword = suggestion;
            found = searchEngine(keyword, dataLines, lineCount, results, 1000);
            if (found > 0)
            {
                cout << "\nFound " << BYELLOW << found << RESET << " results:\n";
                for (int i = 0; i < found; i++)
                    cout << GREEN << i + 1 << ". " << RESET << results[i] << "\n";

                logSearchToFile(keyword, history, frequency, historyCount);
                waitForEnter();
                return;
            }
        }
    }

    // --- FINAL Separate Keyword Handling ---
    // --- FINAL Separate Keyword Handling ---
    // At this point, full phrase didn't match any line

    // --- FINAL Separate Keyword Handling ---
    // At this point the full phrase didn't match any single line.
    // We'll check whether individual words exist and offer the separate-search menu.

    const int MAX_WORDS = 15;
    string keywords[MAX_WORDS];

    // Prefer corrected suggestion if user accepted it, otherwise use original input
    string finalQuery = (correctionMade && !suggestion.empty()) ? suggestion : keyword;

    // Split finalQuery into words
    int keywordCount = splitWords(finalQuery, keywords, MAX_WORDS);

    // If it's just one word, nothing to split — just show no-results
    if (keywordCount <= 1)
    {
        cout << RED << "\nNo results found for \"" << GREEN << finalQuery << RED << "\".\n"
             << RESET;
        waitForEnter();
        return;
    }

    // Check if any of the individual words actually exist in the dataset
    bool anyExists = false;
    for (int i = 0; i < keywordCount; ++i)
    {
        string tempResults[100];
        int cnt = searchEngine(keywords[i], dataLines, lineCount, tempResults, 100);
        if (cnt > 0)
        {
            anyExists = true;
            break;
        }
    }

    // If none of the words appear anywhere, just show no-results (don't offer separate-search)
    if (!anyExists)
    {
        cout << RED << "\nNo results found for \"" << GREEN << finalQuery << RED << "\".\n"
             << RESET;
        waitForEnter();
        return;
    }

    // Now we know at least one word exists somewhere — show the "separate keywords" menu
    cout << RED << "\nNo line contains all keywords in your query.\n"
         << RESET;
    cout << CYAN << "Do you want to search individual keywords separately?\n"
         << RESET;
    for (int i = 0; i < keywordCount; ++i)
    {
        cout << ORANGE << i + 1 << ". " << UNDERLINE << GREEN << keywords[i] << RESET << "\n";
    }
    cout << ORANGE << "0. " << RED << "No, return to main menu\n"
         << RESET;

    // read user's choice and validate
    int choice = -1;
    while (!(cin >> choice) || choice < 0 || choice > keywordCount)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Enter 0-" << keywordCount << ": " << RESET;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // handle choice
    if (choice == 0)
        return;

    string selectedKeyword = keywords[choice - 1];

    // run search for the selected single keyword
    string selResults[1000];
    int selFound = searchEngine(selectedKeyword, dataLines, lineCount, selResults, 1000);

    if (selFound > 0)
    {
        cout << "\n"
             << CYAN << "Results for " << GREEN << "\"" << selectedKeyword << "\"" << RESET << ":\n";
        for (int i = 0; i < selFound; ++i)
            cout << ORANGE << i + 1 << ". " << RESET << selResults[i] << "\n";

        // log and persist this single-keyword search
        logSearchToFile(selectedKeyword, history, frequency, historyCount);
    }
    else
    {
        cout << RED << "\nNo results found for \"" << GREEN << selectedKeyword << RED << "\".\n"
             << RESET;
    }

    waitForEnter();
    return; // done with this search
}

// =================== Display History ===================

void displayHistory(string *&history, int *&frequency, int &historyCount, string *dataLines, int lineCount)
{
    if (historyCount == 0)
    {
        cout << "\nNo search history.\n";
        waitForEnter();
        return;
    }

    cout << "\n==== SEARCH HISTORY ====\n";
    for (int i = 0; i < historyCount; i++)
    {
        int freq = (frequency ? frequency[i] : 0);
        cout << BOLD << i + 1 << RESET << ". "
             << GREEN << history[i] << RESET
             << " (searched "
             << UNDERLINE << PINK << freq << RESET
             << " times)\n";
    }

    cout << "\nEnter number to re-run search or "
         << BOLD << CYAN << "0" << RESET
         << " to return: ";
    int choice;

    while (!(cin >> choice) || choice < 0 || choice > historyCount)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Enter 0-" << historyCount << ": " << RESET;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0)
        return;

    int idx = choice - 1;
    string keyword = history[idx];

    string results[1000];
    int found = searchEngine(keyword, dataLines, lineCount, results, 1000);

    if (found > 0)
    {
        cout << "\nResults for \"" << GREEN << keyword << RESET << "\" ("
             << BYELLOW << found << RESET << "):\n";
        for (int i = 0; i < found; i++)
            cout << ORANGE << i + 1 << ". " << RESET << results[i] << "\n";
    }
    else
    {
        cout << RED << "\nNo results found for \"" << GREEN << keyword << RED << "\".\n"
             << RESET;
    }

    if (frequency)
        frequency[idx]++;

    waitForEnter();
}
