#include "../include/engine.h"
#include <iostream>
#include <limits>
using namespace std;

void clearScreen()
{
    system("cls");
}

void waitForEnter()
{
    cout << "\nPress ENTER to continue...";
    cin.get();
}

void showWelcome()
{
    cout << "==== SIMPLE TEXT ENGINE ====\n";
}

int displayMainMenu()
{
    int choice;
    cout << "\n========== MAIN MENU ==========\n";
    cout << "1. Search Text\n2. View Search History\n3. View File Information\n4. Exit Program\n";
    cout << "Enter choice (1-4): ";
    while (!(cin >> choice) || choice < 1 || choice > 4)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid! Enter 1-4: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void searchMenu(string *dataLines, int lineCount, string *&searchHistory, int &historyCount)
{
    if (!dataLines || lineCount == 0)
    {
        cout << "No files loaded.\n";
        waitForEnter();
        return;
    }

    string keyword;
    cout << "\nEnter keyword: ";
    getline(cin, keyword);
    if (keyword.empty())
    {
        cout << "Empty input not allowed.\n";
        waitForEnter();
        return;
    }

    string results[1000];
    int found = searchEngine(keyword, dataLines, lineCount, results, 1000);

    if (found > 0)
    {
        cout << "\nFound " << found << " results:\n";
        for (int i = 0; i < found; i++)
            cout << i + 1 << ". " << results[i] << "\n";
    }
    else
    {
        // No results found: trigger "Did you mean"
        string suggestion = didYouMean(keyword, dataLines, lineCount);
        if (!suggestion.empty())
        {
            cout << "\nDid you mean \"" << suggestion << "\"? (1 = Yes / 0 = No): ";
            int ans;
            while (!(cin >> ans) || (ans != 0 && ans != 1))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter 1 for Yes, 0 for No: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush input

            if (ans == 1)
            {
                int found2 = searchEngine(suggestion, dataLines, lineCount, results, 1000);
                if (found2 > 0)
                {
                    cout << "\nFound " << found2 << " results for \"" << suggestion << "\":\n";
                    for (int i = 0; i < found2; i++)
                        cout << i + 1 << ". " << results[i] << "\n";
                    keyword = suggestion; // treat suggestion as actual keyword for history
                }
                else
                {
                    cout << "\nNo results found for \"" << suggestion << "\".\n";
                }
            }
            else
            {
                cout << "\nNo such data exists. Returning to main menu.\n";
                return;
            }
        }
        else
        {
            displayNoResultMessage(keyword);
        }
    }

    // Add to history if not duplicate and not empty
    bool exists = false;
    for (int i = 0; i < historyCount; i++)
        if (searchHistory[i] == keyword)
        {
            exists = true;
            break;
        }

    if (!exists)
    {
        string *newHist = new string[historyCount + 1];
        for (int i = 0; i < historyCount; i++)
            newHist[i] = searchHistory[i];
        newHist[historyCount++] = keyword;
        delete[] searchHistory;
        searchHistory = newHist;
        logSearchToFile(keyword);
    }

}

void displayHistory(string *&history, int &historyCount, string *dataLines, int lineCount)
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
        int freq = 0;
        for (int j = 0; j < historyCount; j++)
            if (history[i] == history[j])
                freq++;
        cout << i + 1 << ". " << history[i] << " (searched " << freq << " times)\n";
    }

    cout << "\nEnter number to re-run search or 0 to return: ";
    int choice;
    while (!(cin >> choice) || choice < 0 || choice > historyCount)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Enter 0-" << historyCount << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice > 0 && dataLines && lineCount > 0)
    {
        string results[1000];
        int found = searchEngine(history[choice - 1], dataLines, lineCount, results, 1000);
        if (found > 0)
        {
            cout << "\nResults for \"" << history[choice - 1] << "\":\n";
            for (int i = 0; i < found; i++)
                cout << i + 1 << ". " << results[i] << "\n";
        }
        else
            displayNoResultMessage(history[choice - 1]);
    }
}

void displayResults(string *results, int count)
{
    cout << "\n==== SEARCH RESULTS ====\n";
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << results[i] << "\n";
}

void displayNoResultMessage(const string &keyword)
{
    cout << "\nNo results found for \"" << keyword << "\"\n";
}
