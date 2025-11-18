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
        cout << "No files loaded.\n";
        waitForEnter();
        return;
    }

    string keyword;
    cout << BMAGENTA << "\nEnter keyword: " << RESET;
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
        string suggestion = didYouMean(keyword, dataLines, lineCount);
        int matchPercent = calculateMatchPercentage(keyword, suggestion);
        if (!suggestion.empty())
        {
            cout << "\n"
                 << BYELLOW << "Did you mean "
                 << RESET << "\"" << ORANGE << suggestion << RESET << "\" "
                 << "(" << PINK << matchPercent << RESET << GREY << "% match" << RESET << ")? "
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
                found = searchEngine(suggestion, dataLines, lineCount, results, 1000);
                if (found > 0)
                {
                    cout << "\n"
                         << BCYAN << "Found " << BYELLOW << found << RESET
                         << BCYAN << " results:\n"
                         << RESET;
                    cout << BMAGENTA << "Press ENTER to continue..." << RESET;
                    waitForEnter();
                    for (int i = 0; i < found; i++)
                        cout << GREEN << i + 1 << ". " << RESET << results[i] << "\n";
                    keyword = suggestion;
                }
                else
                {
                    cout << "\nNo results found for \"" << suggestion << "\".\n";
                    waitForEnter();
                    return;
                }
            }
            else
            {
                cout << "\nNo such data exists. Returning to main menu.\n";
                waitForEnter();
                return;
            }
        }
        else
        {
            cout << "\nNo results found for \"" << keyword << "\".\n";
        }
    }

    // Log keyword to history with frequency
    logSearchToFile(keyword, history, frequency, historyCount);

    waitForEnter();
}

// =================== Display History ===================
void displayHistory(string *&history, int *&frequency, int &historyCount,
                    string *dataLines, int lineCount)
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
        cout << "Invalid input! Enter 0-" << historyCount << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0)
        return;

    int idx = choice - 1;
    string keyword = history[idx];

    // Re-run the search for the saved keyword
    string results[1000];
    int found = searchEngine(keyword, dataLines, lineCount, results, 1000);

    if (found > 0)
    {
        cout << "\nResults for \"" << keyword << "\" (" << found << "):\n";
        for (int i = 0; i < found; i++)
            cout << i + 1 << ". " << results[i] << "\n";
    }
    else
    {
        cout << "\n-----------------------------\n";
        cout << "   No results found for: " << keyword << "\n";
        cout << "-----------------------------\n";
    }

    // Ensure frequency array exists
    if (!frequency)
    {
        frequency = new int[historyCount];
        for (int i = 0; i < historyCount; ++i)
            frequency[i] = 0;
    }

    // Update frequency for this keyword
    frequency[idx]++;

    // Update history.txt
    ofstream fout("history.txt", ios::trunc);
    if (fout.is_open())
    {
        for (int i = 0; i < historyCount; ++i)
        {
            fout << history[i] << "|" << frequency[i] << "\n";
        }
        fout.close();
    }
    else
    {
        cout << BRED << "Warning: failed to update history.txt\n"
             << RESET;
    }

    waitForEnter();
}
